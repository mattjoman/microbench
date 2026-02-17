#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "./counter_group.h"
#include "./batch.h"

#define MAX_RATIO_METRICS 2

typedef struct counter_metric {
    int counter_id;
    uint64_t min;
    uint64_t max;
    uint64_t median;
} counter_metric_t;

typedef struct ratio_metric {
    double min;
    double max;
    double median;
} ratio_metric_t;

typedef union metric_union {
    counter_metric_t counter;
    ratio_metric_t ratio;
} metric_union_t;

typedef enum {
    METRIC_TYPE_COUNTER,
    METRIC_TYPE_RATIO,
} metric_type_t;

typedef struct metric {
    metric_type_t metric_type;
    metric_union_t metric;
} metric_t;

typedef struct analysis {
    int n_metrics;
    metric_t metrics[MAX_COUNTER_GRP_SIZE + MAX_RATIO_METRICS];
} analysis_t;

analysis_t run_analysis(batch_t *batch, counter_grp_t ctr_grp);

#endif
