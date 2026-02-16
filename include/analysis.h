#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "./counter_group.h"
#include "./batch.h"

#define MAX_RATIO_AGGS 2

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

typedef union agg_union {
    counter_agg_t counter_agg;
    ratio_agg_t ratio_agg;
} agg_union_t;

typedef enum {
    AGG_TYPE_COUNTER,
    AGG_TYPE_RATIO,
} agg_type_t;

typedef struct aggregate {
    agg_type_t agg_type;
    agg_union_t agg_data;
} aggregate_t;

typedef struct analysis {
    int n_aggs;
    aggregate_t aggregates[MAX_COUNTER_GRP_SIZE + MAX_RATIO_AGGS];
} analysis_t;

analysis_t run_analysis(batch_t *batch, counter_grp_t ctr_grp);

#endif
