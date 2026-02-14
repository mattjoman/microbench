#ifndef EVENT_H
#define EVENT_H

#define MAX_COUNTER_GRP_SIZE 3

typedef struct ctr_grp {
    int id;
    int size;
    int ctr_ids[MAX_COUNTER_GRP_SIZE];
} ctr_grp_t;

enum {
    COUNTER_CPU_CYCLES,
    COUNTER_REF_CPU_CYCLES,
    COUNTER_INSTRUCTIONS,
    COUNTER_CACHE_ACCESSES,
    COUNTER_CACHE_MISSES,
    COUNTER_L1_CACHE_MISSES,
    COUNTER_BRANCH_INSTRUCTIONS,
    COUNTER_BRANCH_MISPREDICTIONS,
    COUNTER_PAGE_FAULTS,
    COUNTER_CPU_CLOCK_NS,
    COUNTER_TASK_CLOCK_NS,
    COUNTER_ALIGNMENT_FAULTS,
    NUMBER_OF_COUNTERS,
};

extern const char *counter_names[NUMBER_OF_COUNTERS];

enum {
    COUNTER_GRP_IPC,
    NUMBER_OF_COUNTER_GRPS,
};

extern const char *counter_grp_names[NUMBER_OF_COUNTER_GRPS];

const ctr_grp_t *get_ctr_grp(int ctr_grp_id);

#endif
