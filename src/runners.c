#include <stdio.h>

#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

void run_bench_1() {
    struct benchmark_results res1, res2;

    init_contiguous_array();
    res1 = bench_perf_event(test_contiguous_array, 10);
    clean_contiguous_array();

    init_scattered_array();
    res2 = bench_perf_event(test_scattered_array, 10);
    clean_scattered_array();

    printf("\n\n");
    printf("Contiguous Array:\n");

    printf("CPU Cycles: %lu\n", res1.values[METRIC_CPU_CYCLES]);
    printf("CPU Instructions: %lu\n", res1.values[METRIC_INSTRUCTIONS]);
    //printf("Stalled Cycles Frontend: %lu\n", res1.values[METRIC_STALLED_CYCLES_FRONTEND]);
    //printf("Stalled Cycles Backend: %lu\n", res1.values[METRIC_STALLED_CYCLES_BACKEND]);
    printf("Cache Accesses: %lu\n", res1.values[METRIC_CACHE_ACCESSES]);
    printf("All Cache Misses: %lu\n", res1.values[METRIC_CACHE_MISSES]);
    printf("L1 Cache Misses: %lu\n", res1.values[METRIC_L1_CACHE_MISSES]);
    printf("Branch Instructions: %lu\n", res1.values[METRIC_BRANCH_INSTRUCTIONS]);
    printf("Branch Mispredictions: %lu\n", res1.values[METRIC_BRANCH_MISPREDICTIONS]);
    printf("Page Faults: %lu\n", res1.values[METRIC_PAGE_FAULTS]);
    printf("CPU time (ns): %lu\n", res1.values[METRIC_CPU_CLOCK_NS]);
    printf("Task time (ns): %lu\n", res1.values[METRIC_TASK_CLOCK_NS]);
    printf("Memory alignment faults: %lu\n", res1.values[METRIC_ALIGNMENT_FAULTS]);

    printf("\n\n");
    printf("Heap int Pointer Array:\n");

    printf("CPU Cycles: %lu\n", res2.values[METRIC_CPU_CYCLES]);
    printf("CPU Instructions: %lu\n", res2.values[METRIC_INSTRUCTIONS]);
    //printf("Stalled Cycles Frontend: %lu\n", res2.values[METRIC_STALLED_CYCLES_FRONTEND]);
    //printf("Stalled Cycles Backend: %lu\n", res2.values[METRIC_STALLED_CYCLES_BACKEND]);
    printf("Cache Accesses: %lu\n", res2.values[METRIC_CACHE_ACCESSES]);
    printf("All Cache Misses: %lu\n", res2.values[METRIC_CACHE_MISSES]);
    printf("L1 Cache Misses: %lu\n", res2.values[METRIC_L1_CACHE_MISSES]);
    printf("Branch Instructions: %lu\n", res2.values[METRIC_BRANCH_INSTRUCTIONS]);
    printf("Branch Mispredictions: %lu\n", res2.values[METRIC_BRANCH_MISPREDICTIONS]);
    printf("Page Faults: %lu\n", res2.values[METRIC_PAGE_FAULTS]);
    printf("CPU time (ns): %lu\n", res2.values[METRIC_CPU_CLOCK_NS]);
    printf("Task time (ns): %lu\n", res2.values[METRIC_TASK_CLOCK_NS]);
    printf("Memory alignment faults: %lu\n", res2.values[METRIC_ALIGNMENT_FAULTS]);

    printf("\n\n");
}

