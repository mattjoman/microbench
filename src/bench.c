#include <unistd.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include "../include/include.h"

#define METRIC_CPU_CYCLES 0
#define METRIC_L1_CACHE_MISSES 1
#define METRIC_BRANCH_MISPREDICTIONS 4
#define METRIC_PAGE_FAULTS 16

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
        case METRIC_L1_CACHE_MISSES:
            pea.type = PERF_TYPE_HW_CACHE;
            pea.config = PERF_COUNT_HW_CACHE_L1D
                | (PERF_COUNT_HW_CACHE_OP_READ << 8)
                | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
            break;
        case METRIC_BRANCH_MISPREDICTIONS:
            pea.type = PERF_TYPE_HARDWARE;
            pea.config = PERF_COUNT_HW_BRANCH_MISSES;
            break;
        case METRIC_PAGE_FAULTS:
            pea.type = PERF_TYPE_SOFTWARE;
            pea.config = PERF_COUNT_SW_PAGE_FAULTS;
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

uint64_t bench_rdtscp(void (*f)(void)) {
    uint64_t start, end;
    start = rdtscp();
    f();
    end = rdtscp();
    return end - start;
}

#define METRICS ((int[]) { \
    METRIC_CPU_CYCLES \
    ,METRIC_L1_CACHE_MISSES \
    ,METRIC_BRANCH_MISPREDICTIONS \
    ,METRIC_PAGE_FAULTS \
})

struct benchmark_results bench_1(void (*f)(void)) {
    struct perf_event_attr attrs[4];
    int fd[4];
    struct benchmark_results res;

    memset(&res, 0, sizeof(struct benchmark_results));

    // create the perf_event_attribute configs for each metric
    for (int i = 0; i < 4; i++)
        attrs[i] = create_perf_config(METRICS[i]);

    // start recording each metric
    for (int i = 0; i < 4; i++) {
        if ((fd[i] = syscall(SYS_perf_event_open, &(attrs[i]), 0, -1, -1, 0)) == -1)
            exit(1);
    }

    // call function to measure
    f();

    // collect results for each metric
    read(fd[0], &res.cpu_cycles, sizeof(uint64_t));
    read(fd[1], &res.l1_cache_misses, sizeof(uint64_t));
    read(fd[2], &res.branch_mispredictions, sizeof(uint64_t));
    read(fd[3], &res.page_faults, sizeof(uint64_t));

    // close file descriptors
    for (int i = 0; i < 4; i++) {
        if (close(fd[i]) == -1)
            exit(1);
    }

    return res;
}

