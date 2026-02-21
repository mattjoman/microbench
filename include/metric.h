#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"
#include "./data_processing.h"

#define MAX_RATIO_METRICS 1

typedef enum {

    /* counter metrics */

    METRIC_CPU_CYCLES,
    METRIC_REF_CPU_CYCLES,
    METRIC_INSTRUCTIONS,
    METRIC_CACHE_ACCESSES,
    METRIC_CACHE_MISSES,
    METRIC_L1_CACHE_ACCESSES,
    METRIC_L1_CACHE_MISSES,
    METRIC_BRANCH_INSTRUCTIONS,
    METRIC_BRANCH_MISPREDICTIONS,
    METRIC_STALLED_CYCLES_FRONTEND,
    METRIC_STALLED_CYCLES_BACKEND,
    METRIC_PAGE_FAULTS,
    METRIC_CPU_CLOCK_NS,
    METRIC_TASK_CLOCK_NS,
    METRIC_ALIGNMENT_FAULTS,

    /* ratio metrics */

    METRIC_INSTRUCTIONS_PER_CYCLE,
    METRIC_L1_CACHE_READ_MISS_RATE,
    METRIC_BRANCH_MISPRED_RATE,
    METRIC_FE_VS_BE_STALL_RATIO,

    NUMBER_OF_METRICS,

} metric_id_t;

extern const char *metric_names[NUMBER_OF_METRICS];


typedef enum {
    METRIC_GRP_IPC,
    METRIC_GRP_L1_CACHE_READS,
    METRIC_GRP_BRANCH,
    METRIC_GRP_STALLED_CYCLES,
    NUMBER_OF_METRIC_GRPS,
} metric_grp_id_t;

typedef struct ratio_conf {
    metric_id_t numerator_id;
    metric_id_t denominator_id;
} ratio_conf_t;

extern const ratio_conf_t ratio_confs[NUMBER_OF_METRICS];

typedef struct counter_metric {
    int id;
    uint64_t raw[MAX_BATCH_SIZE];
    uint64_agg_t agg;
} counter_metric_t;

typedef struct ratio_metric {
    int id;
    double raw[MAX_BATCH_SIZE];
    double_agg_t agg;
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
