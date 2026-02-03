#include <stdio.h>
#include <string.h>

#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

static batch_t batch_init(int warmup_runs, int batch_runs, int event_group_size,
                                int event_group[])
{
    if (batch_runs < 0 || batch_runs > MAX_BENCH_BATCH_SIZE)
        abort();

    if (event_group_size < 0 || event_group_size > MAX_EVENT_GROUP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;
    batch.event_group_size = event_group_size;

    memcpy(batch.event_group, event_group, event_group_size * sizeof(int));

    return batch;
}

void run_bench_1() {

    batch_t batch;
    int event_group[3] = {
        METRIC_CPU_CYCLES, METRIC_L1_CACHE_MISSES, METRIC_INSTRUCTIONS
    };

    batch = batch_init(2, 10, 3, event_group);

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

