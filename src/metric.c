#include <string.h>
#include <stdlib.h>

#include "../include/metric.h"
#include "../include/data_processing.h"

const char *metric_names[NUMBER_OF_METRICS] = {

    /* counter metrics */

    [METRIC_CPU_CYCLES]            = "CPU_CYCLES",
    [METRIC_REF_CPU_CYCLES]        = "REF_CPU_CYCLES",
    [METRIC_INSTRUCTIONS]          = "INSTRUCTIONS",
    [METRIC_CACHE_ACCESSES]        = "CACHE_ACCESSES",
    [METRIC_CACHE_MISSES]          = "CACHE_MISSES",
    [METRIC_L1_CACHE_MISSES]       = "L1_CACHE_MISSES",
    [METRIC_BRANCH_INSTRUCTIONS]   = "BRANCH_INSTRUCTIONS",
    [METRIC_BRANCH_MISPREDICTIONS] = "BRANCH_MISPREDICTIONS",
    [METRIC_PAGE_FAULTS]           = "PAGE_FAULTS",
    [METRIC_CPU_CLOCK_NS]          = "CPU_CLOCK_NS",
    [METRIC_TASK_CLOCK_NS]         = "TASK_CLOCK_NS",
    [METRIC_ALIGNMENT_FAULTS]      = "ALIGNMENT_FAULTS",

    /* ratio metrics */

    [METRIC_INSTRUCTIONS_PER_CYCLE] = "IPC",
};

const ratio_conf_t ratio_confs[NUMBER_OF_METRICS] = {
    [METRIC_INSTRUCTIONS_PER_CYCLE] = {
        .numerator_id = METRIC_INSTRUCTIONS,
        .denominator_id = METRIC_CPU_CYCLES,
    },
};

const metric_grp_t metric_grps[NUMBER_OF_METRIC_GRPS] = {

    [METRIC_GRP_IPC] = {
        .id = METRIC_GRP_IPC,
        .n_counters = MAX_COUNTER_GRP_SIZE,
        .n_ratios = 1,
        .counter_ids = {
            METRIC_CPU_CYCLES,
            METRIC_REF_CPU_CYCLES,
            METRIC_INSTRUCTIONS,
        },
        .ratio_ids = {
            METRIC_INSTRUCTIONS_PER_CYCLE,
        },
    },

};
