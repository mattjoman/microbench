#ifndef METRIC_H
#define METRIC_H

#include "./batch.h"

#define MAX_RATIO_METRICS 1

enum {

    /* counter metrics */

    METRIC_CPU_CYCLES,
    METRIC_REF_CPU_CYCLES,
    METRIC_INSTRUCTIONS,
    METRIC_CACHE_ACCESSES,
    METRIC_CACHE_MISSES,
    METRIC_L1_CACHE_MISSES,
    METRIC_BRANCH_INSTRUCTIONS,
    METRIC_BRANCH_MISPREDICTIONS,
    METRIC_PAGE_FAULTS,
    METRIC_CPU_CLOCK_NS,
    METRIC_TASK_CLOCK_NS,
    METRIC_ALIGNMENT_FAULTS,

    /* metrics */

    METRIC_INSTRUCTIONS_PER_CYCLE,
};

typedef struct counter_metric {
    int id;
    char name[MAX_NAME_LEN];
    uint64_t min;
    uint64_t max;
    uint64_t median;
    uint64_t raw[MAX_BATCH_SIZE];
} counter_metric_t;

typedef struct ratio_metric {
    int id;
    char name[MAX_NAME_LEN];
    double min;
    double max;
    double median;
    double raw[MAX_BATCH_SIZE];
} ratio_metric_t;

typedef struct batch_res {
    int runs;
    int n_ctr_metrics;
    int n_ratio_metrics;
    counter_metric_t ctr_metrics[MAX_COUNTER_GRP_SIZE];
    ratio_metric_t ratio_metrics[MAX_RATIO_METRICS];
} batch_res_t;

batch_res_t init_batch_res(batch_t *batch, counter_grp_t ctr_grp);

#endif
