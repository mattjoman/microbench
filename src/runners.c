#include <stdio.h>

#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

void run_bench_1() {

    struct bench_batch_results batch_results;

    batch_results.runs = MAX_BENCH_BATCH_SIZE;
    batch_results.runs = MAX_EVENT_GROUP_SIZE;

    init_contiguous_array();
    bench_perf_event(&batch_results, test_contiguous_array, 10);
    clean_contiguous_array();

    printf("CPU cycles:     %ld\n", batch_results.metrics[0].values[0]);
    printf("Instructions:   %ld\n", batch_results.metrics[1].values[0]);
    printf("cache accesses: %ld\n", batch_results.metrics[2].values[0]);

    /*
    init_scattered_array();
    res2 = bench_perf_event(test_scattered_array, 10);
    clean_scattered_array();
    */

    printf("\n\n");
    printf("\n\n");
}

