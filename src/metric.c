#include <string.h>

#include "../include/metric.h"
#include "../include/counter_group.h"
#include "../include/analysis.h"

/*
 * XXX: This would be unnecessary if I use the same
 * enum for metrics as for counters (the counter IDs
 * are a subset of the metric IDs).
 */
static counter_metric_t init_counter_metric(int counter_id)
{
    counter_metric_t metric;
    memset(&metric, 0, sizeof(counter_metric_t));

    switch (counter_id) {

        case COUNTER_CPU_CYCLES:
            metric.id = METRIC_CPU_CYCLES;
            strcpy(metric.name, "CPU_CYCLES");
            break;

        case COUNTER_REF_CPU_CYCLES:
            metric.id = METRIC_REF_CPU_CYCLES;
            strcpy(metric.name, "REF_CPU_CYCLES");
            break;

        case COUNTER_INSTRUCTIONS:
            metric.id = METRIC_INSTRUCTIONS;
            strcpy(metric.name, "INSTRUCTIONS");
            break;

        default:
            break;
    }

    return metric;
}

batch_res_t init_batch_res(batch_t *batch, counter_grp_t ctr_grp)
{
    batch_res_t batch_res;

    batch_res.runs = batch->batch_runs;
    batch_res.n_ctr_metrics = ctr_grp.size;
    batch_res.n_ratio_metrics = 1;

    for (int i = 0; i < ctr_grp.size; i++) {

        int counter_id = ctr_grp.counters[i].id;

        uint64_agg_t c_agg = aggregate_uint64(batch->results[counter_id], batch->batch_runs);

        counter_metric_t ctr_metric = init_counter_metric(counter_id);

        ctr_metric.min = c_agg.min;
        ctr_metric.max = c_agg.max;
        ctr_metric.median = c_agg.median;

        memcpy(ctr_metric.raw, batch->results[counter_id], batch->batch_runs * sizeof(uint64_t));

        batch_res.ctr_metrics[i] = ctr_metric;
    }

    ratio_metric_t ratio_metric;

    ratio_metric.id = METRIC_INSTRUCTIONS_PER_CYCLE;
    strcpy(ratio_metric.name, "IPC");

    calc_ratios(ratio_metric.raw, batch->results[COUNTER_INSTRUCTIONS],
                                  batch->results[COUNTER_CPU_CYCLES],
                                  batch->batch_runs);

    double_agg_t r_agg = aggregate_double(ratio_metric.raw, batch->batch_runs);

    ratio_metric.min = r_agg.min;
    ratio_metric.max = r_agg.max;
    ratio_metric.median = r_agg.median;

    batch_res.ratio_metrics[0] = ratio_metric;

    return batch_res;
}
