#include <stdio.h>
#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

void run_bench_1() {
    struct benchmark_results res1, res2;

    res1 = bench_1(test_contiguous_array);
    res2 = bench_1(test_scattered_array);

    printf("\n\n");
    printf("Contiguous Array:\n");

    printf("CPU Cycles: %lu\n", res1.values[METRIC_CPU_CYCLES]);
    printf("L1 Cache Misses: %lu\n", res1.values[METRIC_L1_CACHE_MISSES]);
    printf("Branch Mispredictions: %lu\n", res1.values[METRIC_BRANCH_MISPREDICTIONS]);
    printf("Page Faults: %lu\n", res1.values[METRIC_PAGE_FAULTS]);

    printf("\n\n");
    printf("Heap int Pointer Array:\n");

    printf("CPU Cycles: %lu\n", res2.values[METRIC_CPU_CYCLES]);
    printf("L1 Cache Misses: %lu\n", res2.values[METRIC_L1_CACHE_MISSES]);
    printf("Branch Mispredictions: %lu\n", res2.values[METRIC_BRANCH_MISPREDICTIONS]);
    printf("Page Faults: %lu\n", res2.values[METRIC_PAGE_FAULTS]);

    printf("\n\n");
}

