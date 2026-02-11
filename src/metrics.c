#include "../include/include.h"

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

const event_group_t eg_ipc = {
    .event_group_id = EVENT_GROUP_IPC,
    .size = 3,
    .event_ids[0] = METRIC_CPU_CYCLES,
    .event_ids[1] = METRIC_REF_CPU_CYCLES,
    .event_ids[2] = METRIC_INSTRUCTIONS,
};

const event_group_t *get_event_group(int event_group_id)
{
    switch (event_group_id) {
        case EVENT_GROUP_IPC:
            return &eg_ipc;
        default:
            return NULL;
    }
}
