#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "./internal.h"
#include "../../include/metric_grp.h"

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

double_agg_t aggregate_double(double array[], unsigned long long size)
{
    double_agg_t agg;
    double *array_cpy = calloc(size, sizeof(double));
    if (!array_cpy) {
        perror("Failed to allocate array for double aggregate calculations");
        exit(1);
    }

    memcpy(array_cpy, array, size * sizeof(double));

    qsort(array_cpy, size, sizeof(double), cmp_double);

    memset(&agg, 0, sizeof(double_agg_t));

    agg.min = array_cpy[0];
    agg.max = array_cpy[size - 1];
    agg.median = array_cpy[(size - 1) / 2]; // lower median

    free(array_cpy);
    return agg;
}

void calc_ratios(double results[], const double numerators[],
                                   const double denominators[],
                                   unsigned long long size)
{
    for (unsigned long long i = 0; i < size; i++) {
        results[i] = numerators[i] / denominators[i];
    }
}
