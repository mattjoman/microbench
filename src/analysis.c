#include <string.h>
#include <stdlib.h>

#include "../include/analysis.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;

    if (x < y)
        return -1;

    if (x > y)
        return 1;

    return 0;
}

static int cmp_double(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;

    if (x < y)
        return -1;

    if (x > y)
        return 1;

    return 0;
}

static counter_metric_t build_ctr_metric(uint64_t batch_ctr_results[], int batch_runs)
{
    counter_metric_t metric;
    uint64_t array_cpy[MAX_BATCH_SIZE];

    memcpy(array_cpy, batch_ctr_results, batch_runs * sizeof(uint64_t));

    qsort(array_cpy, batch_runs, sizeof(uint64_t), cmp_uint64);

    memset(&metric, 0, sizeof(counter_metric_t));

    metric.min = array_cpy[0];
    metric.max = array_cpy[batch_runs - 1];
    metric.median = array_cpy[(batch_runs - 1) / 2]; // lower median

    return metric;
}

static ratio_metric_t build_ratio_metric(double ratios[], int batch_runs)
{
    ratio_metric_t metric;
    double array_cpy[MAX_BATCH_SIZE];

    memcpy(array_cpy, ratios, batch_runs * sizeof(double));

    qsort(array_cpy, batch_runs, sizeof(double), cmp_double);

    memset(&metric, 0, sizeof(ratio_metric_t));

    metric.min = array_cpy[0];
    metric.max = array_cpy[batch_runs - 1];
    metric.median = array_cpy[(batch_runs - 1) / 2]; // lower median

    return metric;
}

static void calc_ratios(double results[], uint64_t numerators[],
                                   uint64_t denominators[],
                                   int batch_runs)
{
    for (int i = 0; i < batch_runs; i++) {
        results[i] = numerators[i] / (1.0 * denominators[i]);
    }
}

analysis_t run_analysis(batch_t *batch, counter_grp_t ctr_grp)
{
    analysis_t analysis;
    counter_metric_t ctr_metric;
    ratio_metric_t ratio_metric;
    metric_t metric;
    int metric_idx;

    for (metric_idx = 0; metric_idx < ctr_grp.size; metric_idx++) {

        int ctr_id = ctr_grp.counters[metric_idx].id;

        ctr_metric = build_ctr_metric(batch->results[ctr_id], batch->batch_runs);

        metric.metric_type = METRIC_TYPE_COUNTER;
        metric.metric.counter.counter_id = ctr_id;
        metric.metric.counter.min = ctr_metric.min;
        metric.metric.counter.max = ctr_metric.max;
        metric.metric.counter.median = ctr_metric.median;

        analysis.metrics[metric_idx] = metric;
    }

    double ratios[MAX_BATCH_SIZE];

    switch (ctr_grp.id) {
        case COUNTER_GRP_IPC:
            calc_ratios(ratios, batch->results[COUNTER_INSTRUCTIONS],
                                batch->results[COUNTER_CPU_CYCLES],
                                batch->batch_runs);
            break;
        default:
            break;
    }

    ratio_metric = build_ratio_metric(ratios, batch->batch_runs);

    metric.metric_type = METRIC_TYPE_RATIO;
    metric.metric.ratio.min = ratio_metric.min;
    metric.metric.ratio.max = ratio_metric.max;
    metric.metric.ratio.median = ratio_metric.median;

    analysis.metrics[metric_idx] = metric;

    analysis.n_metrics = 4;

    return analysis;
}
