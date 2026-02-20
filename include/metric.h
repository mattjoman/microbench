#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"

#define MAX_RATIO_METRICS 1

typedef enum {

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

    /* ratio metrics */

    METRIC_INSTRUCTIONS_PER_CYCLE,

    NUMBER_OF_METRICS,

} metric_id_t;

extern const char *metric_names[NUMBER_OF_METRICS];


typedef enum {
    METRIC_GRP_IPC,
    NUMBER_OF_METRIC_GRPS,
} metric_grp_id_t;

typedef struct ratio_conf {
    metric_id_t numerator_id;
    metric_id_t denominator_id;
} ratio_conf_t;

extern const ratio_conf_t ratio_confs[NUMBER_OF_METRICS];

typedef struct counter_metric {
    int id;
    uint64_t min;
    uint64_t max;
    uint64_t median;
    uint64_t raw[MAX_BATCH_SIZE];
} counter_metric_t;

typedef struct ratio_metric {
    int id;
    double min;
    double max;
    double median;
    double raw[MAX_BATCH_SIZE];
} ratio_metric_t;

typedef struct metric_grp {
    int id;
    int n_counters;
    int n_ratios;
    metric_id_t counter_ids[MAX_COUNTER_GRP_SIZE];
    metric_id_t ratio_ids[MAX_COUNTER_GRP_SIZE];
} metric_grp_t;

extern const metric_grp_t metric_grps[NUMBER_OF_METRIC_GRPS];

#endif
