#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "./batch.h"

typedef struct uint64_agg {
    uint64_t min;
    uint64_t max;
    uint64_t median;
} uint64_agg_t;

typedef struct double_agg {
    double min;
    double max;
    double median;
} double_agg_t;

uint64_agg_t aggregate_uint64(uint64_t array[], int size);

double_agg_t aggregate_double(double array[], int size);

void calc_ratios(double results[], uint64_t numerators[],
                                   uint64_t denominators[],
                                   int size);

#endif
