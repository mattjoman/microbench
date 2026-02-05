#include <stdio.h>
#include <string.h>

#include "../include/include.h"

static batch_t batch_init(int warmup_runs, int batch_runs, int event_group_size,
                                                            int event_group[])
{
    if (batch_runs < 1 || batch_runs > MAX_BENCH_BATCH_SIZE)
        abort();

    if (event_group_size < 1 || event_group_size > MAX_EVENT_GROUP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;
    batch.event_group_size = event_group_size;

    memcpy(batch.event_group, event_group, event_group_size * sizeof(int));

    return batch;
}

void experiment_1(void)
{

    batch_t batch;
    int batch_runs;
    int event_group[3] = {
        METRIC_CPU_CYCLES, METRIC_L1_CACHE_MISSES, METRIC_INSTRUCTIONS
    };
    metric_agg_t agg;
    workload_t workload;

    batch_runs = 3;
    batch = batch_init(2, batch_runs, 3, event_group);

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
}

