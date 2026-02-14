#include <stdlib.h>

#include "../include/counter.h"

const char *counter_names[NUMBER_OF_CTRS] = {
    [CTR_CPU_CYCLES]            = "CTR_CPU_CYCLES",
    [CTR_REF_CPU_CYCLES]        = "CTR_REF_CPU_CYCLES",
    [CTR_INSTRUCTIONS]          = "CTR_INSTRUCTIONS",
    [CTR_CACHE_ACCESSES]        = "CTR_CACHE_ACCESSES",
    [CTR_CACHE_MISSES]          = "CTR_CACHE_MISSES",
    [CTR_L1_CACHE_MISSES]       = "CTR_L1_CACHE_MISSES",
    [CTR_BRANCH_INSTRUCTIONS]   = "CTR_BRANCH_INSTRUCTIONS",
    [CTR_BRANCH_MISPREDICTIONS] = "CTR_BRANCH_MISPREDICTIONS",
    [CTR_PAGE_FAULTS]           = "CTR_PAGE_FAULTS",
    [CTR_CPU_CLOCK_NS]          = "CTR_CPU_CLOCK_NS",
    [CTR_TASK_CLOCK_NS]         = "CTR_TASK_CLOCK_NS",
    [CTR_ALIGNMENT_FAULTS]      = "CTR_ALIGNMENT_FAULTS",
};

const char *ctr_grp_names[NUMBER_OF_CTR_GRPS] = {
    [CTR_GRP_IPC]               = "CTR_GRP_IPC",
};

static const ctr_grp_t ctr_grp_ipc = {
    .id = CTR_GRP_IPC,
    .size = MAX_CTR_GRP_SIZE,
    .ctr_ids[0] = CTR_CPU_CYCLES,
    .ctr_ids[1] = CTR_REF_CPU_CYCLES,
    .ctr_ids[2] = CTR_INSTRUCTIONS,
};

const ctr_grp_t *get_ctr_grp(int ctr_grp_id)
{
    switch (ctr_grp_id) {
        case CTR_GRP_IPC:
            return &ctr_grp_ipc;
        default:
            break;
    }

    abort();

    return NULL;
}
