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

static counter_agg_t aggregate_ctr(uint64_t batch_ctr_results[], int batch_runs)
{
    counter_agg_t agg;
    uint64_t array_cpy[MAX_BATCH_SIZE];

    memcpy(array_cpy, batch_ctr_results, batch_runs * sizeof(uint64_t));

    qsort(array_cpy, batch_runs, sizeof(uint64_t), cmp_uint64);

    memset(&agg, 0, sizeof(counter_agg_t));

    agg.min = array_cpy[0];
    agg.max = array_cpy[batch_runs - 1];
    agg.median = array_cpy[(batch_runs - 1) / 2]; // lower median

    return agg;
}

static ratio_agg_t aggregate_ratio(double ratios[], int batch_runs)
{
    ratio_agg_t agg;
    double array_cpy[MAX_BATCH_SIZE];

    memcpy(array_cpy, ratios, batch_runs * sizeof(double));

    qsort(array_cpy, batch_runs, sizeof(double), cmp_double);

    memset(&agg, 0, sizeof(ratio_agg_t));

    agg.min = array_cpy[0];
    agg.max = array_cpy[batch_runs - 1];
    agg.median = array_cpy[(batch_runs - 1) / 2]; // lower median

    return agg;
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
    counter_agg_t ctr_agg;
    ratio_agg_t ratio_agg;

    for (int c = 0; c < ctr_grp.size; c++) {

        int ctr_id = ctr_grp.counters[c].id;

        ctr_agg = aggregate_ctr(batch->results[ctr_id], batch->batch_runs);
        ctr_agg.counter_id = ctr_id;

        analysis.counter_aggs[c] = ctr_agg;
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

    ratio_agg = aggregate_ratio(ratios, batch->batch_runs);

    analysis.ratio_agg = ratio_agg;

    return analysis;
}
