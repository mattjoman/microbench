#include <string.h>
#include <stdlib.h>

#include "../include/metric.h"
#include "../include/data_processing.h"

const char *counter_names[NUMBER_OF_COUNTERS] = {

    /* counter metrics */

    [COUNTER_CPU_CYCLES]              = "CPU_CYCLES",
    [COUNTER_REF_CPU_CYCLES]          = "REF_CPU_CYCLES",
    [COUNTER_INSTRUCTIONS]            = "INSTRUCTIONS",
    [COUNTER_CACHE_ACCESSES]          = "LLC_CACHE_ACCESSES",
    [COUNTER_CACHE_MISSES]            = "LLC_CACHE_MISSES",
    [COUNTER_L1_CACHE_ACCESSES]       = "L1_CACHE_ACCESSES",
    [COUNTER_L1_CACHE_MISSES]         = "L1_CACHE_MISSES",
    [COUNTER_BRANCH_INSTRUCTIONS]     = "BRANCH_INSTRUCTIONS",
    [COUNTER_BRANCH_MISPREDICTIONS]   = "BRANCH_MISPREDICTIONS",
    [COUNTER_STALLED_CYCLES_FRONTEND] = "STALLED_CYCLES_FRONTEND",
    [COUNTER_STALLED_CYCLES_BACKEND]  = "STALLED_CYCLES_BACKEND",
    [COUNTER_PAGE_FAULTS]             = "PAGE_FAULTS",
    [COUNTER_CPU_CLOCK_NS]            = "CPU_CLOCK_NS",
    [COUNTER_TASK_CLOCK_NS]           = "TASK_CLOCK_NS",
    [COUNTER_ALIGNMENT_FAULTS]        = "ALIGNMENT_FAULTS",

};

const ratio_conf_t ratio_confs[NUMBER_OF_RATIOS] = {
    [RATIO_INSTRUCTIONS_PER_CYCLE] = {
        .name = "IPC",
        .numerator_id = COUNTER_INSTRUCTIONS,
        .denominator_id = COUNTER_CPU_CYCLES,
    },
    [RATIO_CYCLES_PER_INSTRUCTION] = {
        .name = "CPI",
        .numerator_id = COUNTER_CPU_CYCLES,
        .denominator_id = COUNTER_INSTRUCTIONS,
    },
    [RATIO_LLC_CACHE_MISS_RATE] = {
        .name = "LLC_MISS_RATE",
        .numerator_id = COUNTER_CACHE_MISSES,
        .denominator_id = COUNTER_CACHE_ACCESSES,
    },
    [RATIO_L1_CACHE_READ_MISS_RATE] = {
        .name = "L1_READ_MISS_RATE",
        .numerator_id = COUNTER_L1_CACHE_MISSES,
        .denominator_id = COUNTER_L1_CACHE_ACCESSES,
    },
    [RATIO_BRANCH_MISPRED_RATE] = {
        .name = "BRANCH_MISPRED_RATE",
        .numerator_id = COUNTER_BRANCH_MISPREDICTIONS,
        .denominator_id = COUNTER_BRANCH_INSTRUCTIONS,
    },
    [RATIO_FE_VS_BE_STALLS] = {
        .name = "FE_VS_BE_STALLS",
        .numerator_id = COUNTER_STALLED_CYCLES_FRONTEND,
        .denominator_id = COUNTER_STALLED_CYCLES_BACKEND,
    },
};

const metric_grp_t metric_grps[NUMBER_OF_METRIC_GRPS] = {

    [METRIC_GRP_IPC] = {
        .id = METRIC_GRP_IPC,
        .n_counters = MAX_COUNTER_GRP_SIZE,
        .n_ratios = 2,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_REF_CPU_CYCLES,
            COUNTER_INSTRUCTIONS,
        },
        .ratio_ids = {
            RATIO_INSTRUCTIONS_PER_CYCLE,
            RATIO_CYCLES_PER_INSTRUCTION,
        },
    },

    [METRIC_GRP_LLC_CACHE] = {
        .id = METRIC_GRP_LLC_CACHE,
        .n_counters = MAX_COUNTER_GRP_SIZE,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_CACHE_ACCESSES,
            COUNTER_CACHE_MISSES,
        },
        .ratio_ids = {
            RATIO_LLC_CACHE_MISS_RATE,
        },
    },

    [METRIC_GRP_L1_CACHE_READS] = {
        .id = METRIC_GRP_L1_CACHE_READS,
        .n_counters = MAX_COUNTER_GRP_SIZE,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_L1_CACHE_ACCESSES,
            COUNTER_L1_CACHE_MISSES,
        },
        .ratio_ids = {
            RATIO_L1_CACHE_READ_MISS_RATE,
        },
    },

    [METRIC_GRP_BRANCH] = {
        .id = METRIC_GRP_BRANCH,
        .n_counters = MAX_COUNTER_GRP_SIZE,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_BRANCH_INSTRUCTIONS,
            COUNTER_BRANCH_MISPREDICTIONS,
        },
        .ratio_ids = {
            RATIO_BRANCH_MISPRED_RATE,
        },
    },

    [METRIC_GRP_STALLED_CYCLES] = {
        .id = METRIC_GRP_STALLED_CYCLES,
        .n_counters = MAX_COUNTER_GRP_SIZE,
        .n_ratios = 1,
        .counter_ids = {
            COUNTER_CPU_CYCLES,
            COUNTER_STALLED_CYCLES_FRONTEND,
            COUNTER_STALLED_CYCLES_BACKEND,
        },
        .ratio_ids = {
            RATIO_FE_VS_BE_STALLS,
        },
    },

};
