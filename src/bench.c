#include <unistd.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include "../include/include.h"




/*** STATIC HELPERS ***/

static uint64_t rdtscp() {
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}

#define METRIC_CPU_CYCLES 0
#define METRIC_L1_CACHE_MISSES 1
#define METRIC_BRANCH_MISPREDICTIONS 4
#define METRIC_PAGE_FAULTS 16

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




/*** BENCHMARKS ***/

uint64_t bench_rdtscp(void (*f)(void)) {
    uint64_t start, end;
    start = rdtscp();
    f();
    end = rdtscp();
    return end - start;
}

uint64_t bench_cache_miss(void (*f)(void)) {
    struct perf_event_attr pea;
    int fd;
    uint64_t count;

    pea = create_perf_config(METRIC_BRANCH_MISPREDICTIONS);

    if ((fd = syscall(SYS_perf_event_open, &pea, 0, -1, -1, 0)) == -1)
        exit(1);
    f();
    read(fd, &count, sizeof(count));

    close(fd);

    return count;
}

