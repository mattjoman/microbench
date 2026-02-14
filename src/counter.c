#include <stdlib.h>

#include "../include/counter.h"

const char *counter_names[NUMBER_OF_COUNTERS] = {
    [COUNTER_CPU_CYCLES]            = "COUNTER_CPU_CYCLES",
    [COUNTER_REF_CPU_CYCLES]        = "COUNTER_REF_CPU_CYCLES",
    [COUNTER_INSTRUCTIONS]          = "COUNTER_INSTRUCTIONS",
    [COUNTER_CACHE_ACCESSES]        = "COUNTER_CACHE_ACCESSES",
    [COUNTER_CACHE_MISSES]          = "COUNTER_CACHE_MISSES",
    [COUNTER_L1_CACHE_MISSES]       = "COUNTER_L1_CACHE_MISSES",
    [COUNTER_BRANCH_INSTRUCTIONS]   = "COUNTER_BRANCH_INSTRUCTIONS",
    [COUNTER_BRANCH_MISPREDICTIONS] = "COUNTER_BRANCH_MISPREDICTIONS",
    [COUNTER_PAGE_FAULTS]           = "COUNTER_PAGE_FAULTS",
    [COUNTER_CPU_CLOCK_NS]          = "COUNTER_CPU_CLOCK_NS",
    [COUNTER_TASK_CLOCK_NS]         = "COUNTER_TASK_CLOCK_NS",
    [COUNTER_ALIGNMENT_FAULTS]      = "COUNTER_ALIGNMENT_FAULTS",
};

const char *counter_grp_names[NUMBER_OF_COUNTER_GRPS] = {
    [COUNTER_GRP_IPC]               = "COUNTER_GRP_IPC",
};

static const ctr_grp_t ctr_grp_ipc = {
    .id = COUNTER_GRP_IPC,
    .size = MAX_COUNTER_GRP_SIZE,
    .ctr_ids[0] = COUNTER_CPU_CYCLES,
    .ctr_ids[1] = COUNTER_REF_CPU_CYCLES,
    .ctr_ids[2] = COUNTER_INSTRUCTIONS,
};

const ctr_grp_t *get_ctr_grp(int ctr_grp_id)
{
    switch (ctr_grp_id) {
        case COUNTER_GRP_IPC:
            return &ctr_grp_ipc;
        default:
            break;
    }

    abort();

    return NULL;
}
