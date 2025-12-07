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
    printf("CPU Cycles: %lu\n", res1.cpu_cycles);
    printf("L1 Cache Misses: %lu\n", res1.l1_cache_misses);
    printf("Branch Mispredictions: %lu\n", res1.branch_mispredictions);
    printf("Page Faults: %lu\n", res1.page_faults);

    printf("\n\n");

    printf("Heap int Pointer Array:\n");
    printf("CPU Cycles: %lu\n", res2.cpu_cycles);
    printf("L1 Cache Misses: %lu\n", res2.l1_cache_misses);
    printf("Branch Mispredictions: %lu\n", res2.branch_mispredictions);
    printf("Page Faults: %lu\n", res2.page_faults);
    printf("\n\n");
}

