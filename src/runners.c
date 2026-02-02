#include <stdio.h>

#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

void run_bench_1() {

    batch_t batch;

    batch.warmup_runs = 100;
    batch.batch_runs = MAX_BENCH_BATCH_SIZE;
    batch.event_group_size = MAX_EVENT_GROUP_SIZE;

    batch.event_group[0] = METRIC_CPU_CYCLES;
    batch.event_group[1] = METRIC_INSTRUCTIONS;
    batch.event_group[2] = METRIC_L1_CACHE_MISSES;

    init_contiguous_array();
    bench_perf_event(&batch, test_contiguous_array);
    clean_contiguous_array();

    printf("\n\n");
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][0]);
    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][0]);
    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][0]);
    printf("\n\n");



    init_scattered_array();
    bench_perf_event(&batch, test_scattered_array);
    clean_scattered_array();

    printf("\n\n");
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][0]);
    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][0]);
    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][0]);
    printf("\n\n");
}

