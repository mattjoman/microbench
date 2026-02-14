#include <stdlib.h>

#include "../include/event.h"

const char *metric_names[NUMBER_OF_METRICS] = {
    [METRIC_CPU_CYCLES]            = "METRIC_CPU_CYCLES",
    [METRIC_REF_CPU_CYCLES]        = "METRIC_REF_CPU_CYCLES",
    [METRIC_INSTRUCTIONS]          = "METRIC_INSTRUCTIONS",
    [METRIC_CACHE_ACCESSES]        = "METRIC_CACHE_ACCESSES",
    [METRIC_CACHE_MISSES]          = "METRIC_CACHE_MISSES",
    [METRIC_L1_CACHE_MISSES]       = "METRIC_L1_CACHE_MISSES",
    [METRIC_BRANCH_INSTRUCTIONS]   = "METRIC_BRANCH_INSTRUCTIONS",
    [METRIC_BRANCH_MISPREDICTIONS] = "METRIC_BRANCH_MISPREDICTIONS",
    [METRIC_PAGE_FAULTS]           = "METRIC_PAGE_FAULTS",
    [METRIC_CPU_CLOCK_NS]          = "METRIC_CPU_CLOCK_NS",
    [METRIC_TASK_CLOCK_NS]         = "METRIC_TASK_CLOCK_NS",
    [METRIC_ALIGNMENT_FAULTS]      = "METRIC_ALIGNMENT_FAULTS",
};

const char *event_group_names[NUMBER_OF_EVENT_GROUPS] = {
    [EVENT_GROUP_IPC]               = "EVENT_GROUP_IPC",
    //[EVENT_GROUP_FEND_VS_BEND]      = "EVENT_GROUP_FEND_VS_BEND",
    //[EVENT_GROUP_BRANCH_EFFICIENCY] = "EVENT_GROUP_BRANCH_EFFICIENCY",
};

static const event_group_t egroup_ipc = {
    .id = EVENT_GROUP_IPC,
    .size = MAX_EVENT_GROUP_SIZE,
    .event_ids[0] = METRIC_CPU_CYCLES,
    .event_ids[1] = METRIC_REF_CPU_CYCLES,
    .event_ids[2] = METRIC_INSTRUCTIONS,
};

const event_group_t *get_event_group(int event_group_id)
{
    switch (event_group_id) {
        case EVENT_GROUP_IPC:
            return &egroup_ipc;
        default:
            break;
    }

    abort();

    return NULL;
}
