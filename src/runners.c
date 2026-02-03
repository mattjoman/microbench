#include <stdio.h>

#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

void run_bench_1() {

    batch_t batch;

    batch.warmup_runs      = 3;
    batch.batch_runs       = 3;
    batch.event_group_size =  3;

    batch.event_group[0] = METRIC_CPU_CYCLES;
    batch.event_group[1] = METRIC_L1_CACHE_MISSES;
    batch.event_group[2] = METRIC_INSTRUCTIONS;

    init_contiguous_array();
    bench_perf_event(&batch, test_contiguous_array);
    clean_contiguous_array();

    printf("\n\n");
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][0]);
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][1]);
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][2]);

    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][0]);
    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][1]);
    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][2]);

    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][0]);
    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][1]);
    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][2]);

    printf("\n\n");


    /*

    init_scattered_array();
    bench_perf_event(&batch, test_scattered_array);
    clean_scattered_array();

    printf("\n\n");
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][0]);
    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][0]);
    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][0]);
    printf("\n\n");
    */
}

