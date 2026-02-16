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
#include <assert.h>

#include "../include/bench.h"

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
        case COUNTER_CPU_CYCLES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CPU_CYCLES;
            break;
        case COUNTER_REF_CPU_CYCLES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_REF_CPU_CYCLES;
            break;
        case COUNTER_INSTRUCTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_INSTRUCTIONS;
            break;
        case COUNTER_CACHE_ACCESSES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CACHE_REFERENCES;
            break;
        case COUNTER_CACHE_MISSES:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_CACHE_MISSES;
            break;
        case COUNTER_L1_CACHE_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1D
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case COUNTER_BRANCH_INSTRUCTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
            break;
        case COUNTER_BRANCH_MISPREDICTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_MISSES;
            break;
        case COUNTER_PAGE_FAULTS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_PAGE_FAULTS;
            break;
        case COUNTER_CPU_CLOCK_NS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_CPU_CLOCK;
            break;
        case COUNTER_TASK_CLOCK_NS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_TASK_CLOCK;
            break;
        case COUNTER_ALIGNMENT_FAULTS:
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

static void open_perf_counters(struct perf_event_attr attrs[],
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

/* matches kernel abi */
typedef struct perf_result {
    uint64_t nr;
    uint64_t time_enabled;
    uint64_t time_running;
    struct {
        uint64_t value;
        uint64_t id;
    } values[MAX_COUNTER_GRP_SIZE];
} perf_result_t;

static void store_perf_results(batch_t *batch, perf_result_t perf_results[],
                                               counter_grp_t ctr_grp)
{
    for (int run = 0; run < batch->batch_runs; run++) {

        perf_result_t perf_result = perf_results[run];

        for (unsigned int pr_idx = 0; pr_idx < perf_result.nr; pr_idx++) {

            int counter_id = ctr_grp.counters[pr_idx].id;
            uint64_t value = perf_result.values[pr_idx].value;

            batch->results[counter_id][run] = value;
        }
    }
}

/*
 * This is just a placeholder for now.
 * Might make this function usable at some point.
 */
uint64_t bench_rdtscp(void (*test_func)(void))
{
    uint64_t start, end;
    start = rdtscp();
    test_func();
    end = rdtscp();
    return end - start;
}

int bench_perf_event(batch_t *batch, void (*workload)(void),
                                     counter_grp_t ctr_grp)
{
    struct perf_event_attr attrs[MAX_COUNTER_GRP_SIZE];
    int                    perf_ctr_fds[MAX_COUNTER_GRP_SIZE];
    uint64_t               perf_ctr_ids[MAX_COUNTER_GRP_SIZE];

    perf_result_t perf_results[MAX_BATCH_SIZE];

    for (int i = 0; i < ctr_grp.size; i++) {

        int counter_id = ctr_grp.counters[i].id;
        attrs[i] = create_perf_config(counter_id);
    }

    open_perf_counters(attrs, perf_ctr_fds, perf_ctr_ids, ctr_grp.size);

    pin_thread();

    for (int i = 0; i < batch->warmup_runs; i++)
        workload();

    /*
     * This is the main benchmark loop.
     * Keep it as clean and minimal as possible
     * to reduce noise.
     */
    for (int run = 0; run < batch->batch_runs; run++) {

        ioctl(perf_ctr_fds[0], PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
        ioctl(perf_ctr_fds[0], PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

        asm volatile("" ::: "memory");

        workload();

        asm volatile("" ::: "memory");

        ioctl(perf_ctr_fds[0], PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

        read(perf_ctr_fds[0], &perf_results[run], sizeof(perf_result_t));
    }

    for (int i = 0; i < ctr_grp.size; i++) {
        if (close(perf_ctr_fds[i]) == -1)
            exit(1);
    }

    for (int i = 0; i < ctr_grp.size; i++)
        assert(perf_results[0].values[i].id == perf_ctr_ids[i]);

    store_perf_results(batch, perf_results, ctr_grp);

    return 0;
}
