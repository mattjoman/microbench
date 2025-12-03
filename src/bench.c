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

    memset(&pea, 0, sizeof(struct perf_event_attr));

    pea.type = PERF_TYPE_HW_CACHE;
    pea.config = PERF_COUNT_HW_CACHE_L1D
        | (PERF_COUNT_HW_CACHE_OP_READ << 8)
        | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16);
    pea.size = sizeof(struct perf_event_attr);
    pea.disabled = 0;
    pea.exclude_kernel = 1;
    pea.exclude_hv = 1;
    pea.exclude_idle = 1;

    fd = syscall(SYS_perf_event_open, &pea, 0, -1, -1, 0);

    f();

    read(fd, &count, sizeof(count));
    close(fd);
    return count;
}

