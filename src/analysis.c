#include <string.h>

#include "../include/include.h"

static void sort_swap(uint64_t *a, uint64_t *b)
{
    uint64_t tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static int sort_partition(uint64_t array[], int low, int high)
{
    uint64_t pivot;
    int i, j;

    i = low - 1;
    pivot = array[high];

    for (j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            sort_swap(&array[i], &array[j]);
        }
    }

    sort_swap(&array[i + 1], &array[high]);

    return i + 1;
}

static void sort(uint64_t array[], int low, int high)
{
    int p;

    if (low < high) {
        p = sort_partition(array, low, high);
        sort(array, low, p - 1);
        sort(array, p + 1, high);
    }
}

static metric_agg_t metric_agg(uint64_t batch_metric_results[], int batch_runs, int event_id)
{
    metric_agg_t agg;
    uint64_t array_cpy[MAX_BENCH_BATCH_SIZE];

    memcpy(array_cpy, batch_metric_results, batch_runs * sizeof(uint64_t));

    sort(array_cpy, 0, batch_runs - 1);

    memset(&agg, 0, sizeof(metric_agg_t));

    agg.event_id = event_id;
    agg.min = array_cpy[0];
    agg.max = array_cpy[batch_runs - 1];
    agg.median = array_cpy[(batch_runs - 1) / 2]; // lower median
    //agg.median = array_cpy[batch_runs / 2]; // upper median

    return agg;
}







static void r_sort_swap(double *a, double *b)
{
    double tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static int r_sort_partition(double array[], int low, int high)
{
    double pivot;
    int i, j;

    i = low - 1;
    pivot = array[high];

    for (j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            r_sort_swap(&array[i], &array[j]);
        }
    }

    r_sort_swap(&array[i + 1], &array[high]);

    return i + 1;
}

static void r_sort(double array[], int low, int high)
{
    int p;

    if (low < high) {
        p = r_sort_partition(array, low, high);
        r_sort(array, low, p - 1);
        r_sort(array, p + 1, high);
    }
}

static ratio_agg_t ratio_agg(double ratios[], int batch_runs)
{
    ratio_agg_t agg;
    double array_cpy[MAX_BENCH_BATCH_SIZE];

    memcpy(array_cpy, ratios, batch_runs * sizeof(double));

    r_sort(array_cpy, 0, batch_runs - 1);

    memset(&agg, 0, sizeof(ratio_agg_t));
    agg.min = array_cpy[0];
    agg.max = array_cpy[batch_runs - 1];
    agg.median = array_cpy[(batch_runs - 1) / 2]; // lower median
    //agg.median = array_cpy[batch_runs / 2]; // upper median

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

analysis_t run_analysis(batch_t *batch, event_group_t egroup)
{
    analysis_t analysis;
    metric_agg_t e_agg;
    ratio_agg_t r_agg;

    for (int e = 0; e < MAX_EVENT_GROUP_SIZE; e++) {

        int event_id = egroup.event_ids[e];
        e_agg = metric_agg(batch->results[event_id], batch->batch_runs,
                                                     event_id);

        analysis.event_aggs[e] = e_agg;
    }

    double ratios[MAX_BENCH_BATCH_SIZE];

    switch (egroup.id) {
        case EVENT_GROUP_IPC:
            calc_ratios(ratios, batch->results[METRIC_INSTRUCTIONS],
                                batch->results[METRIC_CPU_CYCLES],
                                batch->batch_runs);
            break;
        default:
            break;
    }

    r_agg = ratio_agg(ratios, batch->batch_runs);

    analysis.ratio_agg = r_agg;

    return analysis;
}



































