#include <string.h>
#include <stdlib.h>

#include "../include/metric.h"
#include "../include/data_processing.h"

static metric_grp_t get_metric_grp(metric_grp_id_t id)
{
    switch (id) {

        case METRIC_GRP_IPC:
            metric_grp_t metric_grp = {
                .id = METRIC_GRP_IPC,
                .n_counters = 3,
                .n_ratios = 1,
                .counter_ids = {
                    METRIC_CPU_CYCLES,
                    METRIC_REF_CPU_CYCLES,
                    METRIC_INSTRUCTIONS,
                },
                .ratio_ids = {
                    METRIC_INSTRUCTIONS_PER_CYCLE,
                },
            };
            return metric_grp;

        default:
            const metric_grp_t null_grp = { 0 };
            return null_grp;
    }
}

batch_metrics_t init_batch_metrics(int warmup_runs, int batch_runs, metric_grp_id_t id)
{
    metric_grp_t metric_grp = get_metric_grp(id);

    if (batch_runs < 1 || batch_runs > MAX_BATCH_SIZE)
        abort();

    if (metric_grp.n_counters < 1 || metric_grp.n_counters > MAX_COUNTER_GRP_SIZE)
        abort();

    batch_metrics_t batch_metrics;

    batch_metrics.metric_grp = metric_grp;
    batch_metrics.warmup_runs = warmup_runs;
    batch_metrics.batch_runs = batch_runs;

    for (int i = 0; i < metric_grp.n_counters; i++) {

        counter_metric_t counter;
        counter.id = metric_grp.counter_ids[i];

        batch_metrics.counters[i] = counter;
    }

    for (int i = 0; i < metric_grp.n_ratios; i++) {

        ratio_metric_t ratio;
        ratio.id = metric_grp.ratio_ids[i];

        batch_metrics.ratios[i] = ratio;
    }

    return batch_metrics;
}

void process_batch_metrics(batch_metrics_t *batch_metrics)
{
    int batch_runs = batch_metrics->batch_runs;

    metric_id_t id_map[NUMBER_OF_METRICS];

    for (int i = 0; i < batch_metrics->metric_grp.n_counters; i++) {

        uint64_agg_t c_agg = aggregate_uint64(batch_metrics->counters[i].raw, batch_runs);

        batch_metrics->counters[i].min = c_agg.min;
        batch_metrics->counters[i].max = c_agg.max;
        batch_metrics->counters[i].median = c_agg.median;

        id_map[batch_metrics->counters[i].id] = i;
    }

    calc_ratios(batch_metrics->ratios[0].raw, batch_metrics->counters[id_map[METRIC_INSTRUCTIONS]].raw,
                                              batch_metrics->counters[id_map[METRIC_CPU_CYCLES]].raw,
                                              batch_runs);

    double_agg_t r_agg = aggregate_double(batch_metrics->ratios[0].raw, batch_runs);

    batch_metrics->ratios[0].min = r_agg.min;
    batch_metrics->ratios[0].max = r_agg.max;
    batch_metrics->ratios[0].median = r_agg.median;
}
