#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "./event.h"
#include "./experiment.h"

typedef struct metric_agg {
    int event_id;
    uint64_t min;
    uint64_t max;
    uint64_t median;
} metric_agg_t;

typedef struct ratio_agg {
    double min;
    double max;
    double median;
} ratio_agg_t;

typedef struct analysis {
    metric_agg_t event_aggs[MAX_EVENT_GROUP_SIZE];
    ratio_agg_t ratio_agg;
} analysis_t;

analysis_t run_analysis(batch_t *batch, event_group_t egroup);

#endif
