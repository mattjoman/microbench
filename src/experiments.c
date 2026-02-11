#include <stdio.h>
#include <string.h>

#include "../include/include.h"

static batch_t batch_init(int warmup_runs, int batch_runs,
                                           event_group_t event_group)
{
    if (batch_runs < 1 || batch_runs > MAX_BENCH_BATCH_SIZE)
        abort();

    if (event_group.size < 1 || event_group.size > MAX_EVENT_GROUP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;
    batch.event_group_size = event_group.size;

    memcpy(batch.event_group, event_group.event_ids,
                              event_group.size * sizeof(int));

    return batch;
}

void experiment_1(void)
{
    batch_t batch;
    int batch_runs;
    metric_agg_t agg;
    workload_t workload;
    event_group_t event_group;

    event_group = *get_event_group(EVENT_GROUP_IPC);

    batch_runs = 10;
    batch = batch_init(2, batch_runs, event_group);

    workload = *get_workload(WL_CONTIGUOUS_ARRAY);
    //workload = *get_workload(WL_SCATTERED_ARRAY);

    workload.init();
    bench_perf_event(&batch, workload.workload);
    workload.clean();

    for (int evt_idx = 0; evt_idx < batch.event_group_size; evt_idx++) {
        int event = batch.event_group[evt_idx];
        agg = metric_agg(batch.results[event], batch.batch_runs);
        printf("\n");
        printf("%s\n", metric_names[event]);
        printf("min:    %ld\n", agg.min);
        printf("max:    %ld\n", agg.max);
        printf("median: %ld\n", agg.median);
        printf("\n");
    }

    double ipcs[MAX_BENCH_BATCH_SIZE];
    calc_ratios(ipcs, batch.results[METRIC_INSTRUCTIONS],
                      batch.results[METRIC_CPU_CYCLES],
                      batch_runs);
    ratio_agg_t r_agg = ratio_agg(ipcs, batch_runs);

    printf("\n");
    printf("IPC\n");
    printf("min:    %f\n", r_agg.min);
    printf("max:    %f\n", r_agg.max);
    printf("median: %f\n", r_agg.median);
    printf("\n");
}
