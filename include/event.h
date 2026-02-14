#ifndef EVENT_H
#define EVENT_H

#define MAX_EVENT_GROUP_SIZE 3

typedef struct event_group {
    int id;
    int size;
    int event_ids[MAX_EVENT_GROUP_SIZE];
} event_group_t;

enum {
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
    NUMBER_OF_METRICS,
};

extern const char *metric_names[NUMBER_OF_METRICS];

enum {
    EVENT_GROUP_IPC,
    //EVENT_GROUP_FEND_VS_BEND,
    //EVENT_GROUP_BRANCH_EFFICIENCY,
    NUMBER_OF_EVENT_GROUPS,
};

extern const char *event_group_names[NUMBER_OF_EVENT_GROUPS];

const event_group_t *get_event_group(int event_group_id);

#endif
