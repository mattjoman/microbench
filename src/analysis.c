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

metric_agg_t metric_agg(uint64_t batch_metric_results[], int batch_runs)
{
    metric_agg_t agg;
    uint64_t array_cpy[MAX_BENCH_BATCH_SIZE];

    memcpy(array_cpy, batch_metric_results, batch_runs * sizeof(uint64_t));

    sort(array_cpy, 0, batch_runs - 1);

    memset(&agg, 0, sizeof(metric_agg_t));
    agg.min = array_cpy[0];
    agg.max = array_cpy[batch_runs - 1];
    agg.median = array_cpy[(batch_runs - 1) / 2]; // lower median
    //agg.median = array_cpy[batch_runs / 2]; // upper median

    return agg;
}

void calc_ratios(double results[], uint64_t numerators[],
                                   uint64_t denominators[],
                                   int batch_runs)
{
    for (int i = 0; i < batch_runs; i++) {
        results[i] = numerators[i] / (1.0 * denominators[i]);
    }
}
