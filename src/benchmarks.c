#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include "../include/include.h"

/* matches kernel abi */
typedef struct run_result {
    uint64_t nr;
    uint64_t time_enabled;
    uint64_t time_running;
    struct {
        uint64_t value;
        uint64_t id;
    } values[MAX_EVENT_GROUP_SIZE];
} run_result_t;

typedef struct event_map {
    int data[MAX_EVENT_GROUP_SIZE];
} event_map_t;

static void pin_thread(void)
{
    cpu_set_t cpuset;
    int core_id = 0;
    
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) == -1)
        exit(1);
}

static uint64_t rdtscp()
{
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}

static struct perf_event_attr create_perf_config(int metric)
{
    struct perf_event_attr pea;

    memset(&pea, 0, sizeof(struct perf_event_attr));

    switch (metric) {
        case METRIC_CPU_CYCLES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CPU_CYCLES;
            break;
        case METRIC_INSTRUCTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_INSTRUCTIONS;
            break;
        case METRIC_CACHE_ACCESSES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CACHE_REFERENCES;
            break;
        case METRIC_CACHE_MISSES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CACHE_MISSES;
            break;
        case METRIC_L1_CACHE_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1D
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_BRANCH_INSTRUCTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
            break;
        case METRIC_BRANCH_MISPREDICTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_MISSES;
            break;
        case METRIC_PAGE_FAULTS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_PAGE_FAULTS;
            break;
        case METRIC_CPU_CLOCK_NS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_CPU_CLOCK;
            break;
        case METRIC_TASK_CLOCK_NS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_TASK_CLOCK;
            break;
        case METRIC_ALIGNMENT_FAULTS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_ALIGNMENT_FAULTS;
            break;
        default:
            break;
    }
    pea.size = sizeof(struct perf_event_attr);
    pea.disabled = 1;
    pea.exclude_kernel = 1;
    pea.exclude_hv = 1;
    pea.exclude_idle = 1;
    pea.read_format = PERF_FORMAT_GROUP |
        PERF_FORMAT_ID |
        PERF_FORMAT_TOTAL_TIME_ENABLED |
        PERF_FORMAT_TOTAL_TIME_RUNNING;

    return pea;
}

struct event_map calculate_event_map(run_result_t run_result,
                                uint64_t counter_ids[], int event_group_size)
{
    event_map_t event_map;

    for (int rr_idx = 0; rr_idx < event_group_size; rr_idx++) {
        for (int cid_idx = 0; cid_idx < event_group_size; cid_idx++) {
            if (run_result.values[rr_idx].id == counter_ids[cid_idx])
                event_map.data[rr_idx] = cid_idx;
        }
    }

    return event_map;
}

static void perf_open_counters(struct perf_event_attr attrs[],
        int counter_fds[], uint64_t counter_ids[], int event_group_size)
{
    counter_fds[0] = syscall(SYS_perf_event_open, &(attrs[0]), 0, -1, -1, 0);

    if (counter_fds[0] == -1)
        exit(1);

    ioctl(counter_fds[0], PERF_EVENT_IOC_ID, &counter_ids[0]);

    for (int evt_idx = 1; evt_idx < event_group_size; evt_idx++) {

        counter_fds[evt_idx] = syscall(SYS_perf_event_open,
                                &(attrs[evt_idx]), 0, -1, counter_fds[0], 0);

        if (counter_fds[evt_idx] == -1)
            exit(1);

        ioctl(counter_fds[evt_idx], PERF_EVENT_IOC_ID, &counter_ids[evt_idx]);
    }
}

static void perf_store_results(batch_t *batch, run_result_t run_results[],
                                                        uint64_t counter_ids[])
{
    event_map_t event_map = calculate_event_map(run_results[0], counter_ids,
                                                    batch->event_group_size);

    for (int run_idx = 0; run_idx < batch->batch_runs; run_idx++) {

        for (int evt_idx = 0; evt_idx < batch->event_group_size; evt_idx++) {

            uint64_t value = run_results[run_idx].values[evt_idx].value;
            int batch_evt_idx = event_map.data[evt_idx];

            batch->results[batch->event_group[batch_evt_idx]][run_idx] = value;
        }
    }
}

uint64_t bench_rdtscp(void (*test_func)(void))
{
    uint64_t start, end;
    start = rdtscp();
    test_func();
    end = rdtscp();
    return end - start;
}

int bench_perf_event(batch_t *batch, void (*workload)(void))
{
    struct perf_event_attr attrs[MAX_EVENT_GROUP_SIZE];
    int counter_fds[MAX_EVENT_GROUP_SIZE];
    uint64_t counter_ids[MAX_EVENT_GROUP_SIZE];
    run_result_t run_results[MAX_BENCH_BATCH_SIZE];

    for (int evt_idx = 0; evt_idx < batch->event_group_size; evt_idx++)
        attrs[evt_idx] = create_perf_config(batch->event_group[evt_idx]);

    perf_open_counters(attrs, counter_fds, counter_ids,
                                                    batch->event_group_size);

    pin_thread();

    for (int wu_num = 0; wu_num < batch->warmup_runs; wu_num++)
        workload();

    for (int run_num = 0; run_num < batch->batch_runs; run_num++) {

        ioctl(counter_fds[0], PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
        ioctl(counter_fds[0], PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

        asm volatile("" ::: "memory");

        workload();

        asm volatile("" ::: "memory");

        ioctl(counter_fds[0], PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

        read(counter_fds[0], &run_results[run_num], sizeof(run_result_t));
    }

    for (int evt_idx = 0; evt_idx < batch->event_group_size; evt_idx++) {
        if (close(counter_fds[evt_idx]) == -1)
            exit(1);
    }

    perf_store_results(batch, run_results, counter_ids);

    return 0;
}
