#include <unistd.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include "../include/include.h"


/*** ====================== STATIC HELPERS ====================== ***/

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
    pea.disabled = 0;
    pea.exclude_kernel = 1;
    pea.exclude_hv = 1;
    pea.exclude_idle = 1;

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

struct benchmark_results bench_perf_event(void (*test_func)(void), unsigned int warmup_runs) {
    struct benchmark_results res;
    unsigned int i;
    struct perf_event_attr attrs[NUMBER_OF_METRICS];
    int fd[NUMBER_OF_METRICS];

    memset(&res, 0, sizeof(struct benchmark_results));

    for (i = 0; i < NUMBER_OF_METRICS; i++)
        attrs[i] = create_perf_config(METRICS[i]);

    for (i = 0; i < warmup_runs; i++)
        test_func();

    for (i = 0; i < NUMBER_OF_METRICS; i++) {
        if ((fd[i] = syscall(SYS_perf_event_open, &(attrs[i]), 0, -1, -1, 0)) == -1)
            exit(1);
    }

    test_func();

    for (i = 0; i < NUMBER_OF_METRICS; i++)
        read(fd[i], &res.values[METRICS[i]], sizeof(uint64_t));

    for (int i = 0; i < 4; i++) {
        if (close(fd[i]) == -1)
            exit(1);
    }

    return res;
}

