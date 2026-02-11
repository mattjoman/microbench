/*
 * This file will contain code for displaying the results of
 * experiments & analysis
 */

#include <stdio.h>

#include "../include/include.h"

static void report_events(batch_t *batch)
{
    metric_agg_t agg;

    for (int evt_idx = 0; evt_idx < batch->event_group_size; evt_idx++) {

        int event = batch->event_group[evt_idx];

        agg = metric_agg(batch->results[event], batch->batch_runs);

        printf("\n");
        printf("%s\n", metric_names[event]);
        printf("min:    %ld\n", agg.min);
        printf("max:    %ld\n", agg.max);
        printf("median: %ld\n", agg.median);
        printf("\n");
    }
}

static void report_ipc(batch_t *batch)
{
    double ipcs[MAX_BENCH_BATCH_SIZE];
    calc_ratios(ipcs, batch->results[METRIC_INSTRUCTIONS],
                      batch->results[METRIC_CPU_CYCLES],
                      batch->batch_runs);

    ratio_agg_t r_agg = ratio_agg(ipcs, batch->batch_runs);

    printf("\n");
    printf("IPC\n");
    printf("min:    %f\n", r_agg.min);
    printf("max:    %f\n", r_agg.max);
    printf("median: %f\n", r_agg.median);
    printf("\n");
}

void report_batch(batch_t *batch, int event_group_id) {
    report_events(batch);

    switch (event_group_id) {
        case EVENT_GROUP_IPC:
            report_ipc(batch);
            break;
        default:
            break;
    }
}
