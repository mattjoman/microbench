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

    batch.event_group[0] = METRIC_INSTRUCTIONS;
    batch.event_group[1] = METRIC_PAGE_FAULTS;
    batch.event_group[2] = METRIC_BRANCH_INSTRUCTIONS;

    init_contiguous_array();
    bench_perf_event(&batch, test_contiguous_array);
    clean_contiguous_array();

    printf("CPU cycles:     %ld\n", batch.results[METRIC_CPU_CYCLES][0]);
    printf("Instructions:   %ld\n", batch.results[METRIC_PAGE_FAULTS][0]);
    printf("cache accesses: %ld\n",
                                batch.results[METRIC_BRANCH_INSTRUCTIONS][0]);

    /*
    init_scattered_array();
    res2 = bench_perf_event(test_scattered_array, 10);
    clean_scattered_array();
    */

    printf("\n\n");
    printf("\n\n");
}

