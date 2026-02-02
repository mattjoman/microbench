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

// matches kernel abi
struct bench_run_results {
    uint64_t nr;
    uint64_t time_enabled;
    uint64_t time_running;
    struct {
        uint64_t value;
        uint64_t id;
    } values[MAX_EVENT_GROUP_SIZE];
};

/*** ====================== STATIC HELPERS ====================== ***/

static void pin_thread() {
    cpu_set_t cpuset;
    int core_id = 0;
    
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) == -1)
        exit(-1);
}

static uint64_t rdtscp() {
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}

static struct perf_event_attr create_perf_config(int metric) {
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
        /*
        case METRIC_STALLED_CYCLES_FRONTEND:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND;
            break;
        case METRIC_STALLED_CYCLES_BACKEND:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND;
            break;
        */
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

/*** ====================== BENCHMARKS ====================== ***/

uint64_t bench_rdtscp(void (*test_func)(void)) {
    uint64_t start, end;
    start = rdtscp();
    test_func();
    end = rdtscp();
    return end - start;
}




int bench_perf_event(batch_t *batch, void (*test_func)(void)) {

    struct perf_event_attr attrs[MAX_EVENT_GROUP_SIZE];
    int fd[MAX_EVENT_GROUP_SIZE];
    uint64_t counter_ids[MAX_EVENT_GROUP_SIZE];
    struct bench_run_results run_results;
    int i;

    memset(&run_results, 0, sizeof(struct bench_run_results));




    // generate configs for each event counter
    for (i = 0; i < batch->event_group_size; i++)
        attrs[i] = create_perf_config(batch->event_group[i]);

    // open event group leader
    fd[0] = syscall(SYS_perf_event_open, &(attrs[0]), 0, -1, -1, 0);
    if (fd[0] == -1) exit(1);
    ioctl(fd[0], PERF_EVENT_IOC_ID, &counter_ids[0]);

    // open the other event counters
    for (i = 1; i < batch->event_group_size; i++) {
        fd[i] = syscall(SYS_perf_event_open, &(attrs[i]), 0, -1, fd[0], 0);
        if (fd[i] == -1) exit(1);

        ioctl(fd[i], PERF_EVENT_IOC_ID, &counter_ids[i]);
    }


    pin_thread();

    // warm up caches, train branch predictor
    for (i = 0; i < batch->warmup_runs; i++)
        test_func();




    // start event counters
    ioctl(fd[0], PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    ioctl(fd[0], PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

    test_func();

    // stop event counters
    ioctl(fd[0], PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);





    // read event counters
    read(fd[0], &run_results, sizeof(struct bench_run_results));




    // close event counters
    for (i = 0; i < batch->event_group_size; i++) {
        if (close(fd[i]) == -1)
            exit(1);
    }




    for (i = 0; i < batch->event_group_size; i++) {
        uint64_t perf_counter_id = run_results.values[i].id;
        uint64_t perf_counter_value = run_results.values[i].value;

        for (int j = 0; j < batch->event_group_size; j++) {
            if (counter_ids[j] == perf_counter_id) {
                batch->results[batch->event_group[j]][0] = perf_counter_value;
            }
        }
    }

    return 1;
}
