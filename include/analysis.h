#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "./counter_group.h"
#include "./batch.h"

typedef struct counter_agg {
    int counter_id;
    uint64_t min;
    uint64_t max;
    uint64_t median;
} counter_agg_t;

typedef struct ratio_agg {
    double min;
    double max;
    double median;
} ratio_agg_t;

typedef struct analysis {
    counter_agg_t counter_aggs[MAX_COUNTER_GRP_SIZE];
    ratio_agg_t ratio_agg;
} analysis_t;

analysis_t run_analysis(batch_t *batch, counter_grp_t ctr_grp);

#endif
