#ifndef COUNTER_GROUP_H
#define COUNTER_GROUP_H

#include "./microbench.h"

#define MAX_COUNTER_GRP_SIZE 3

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

typedef struct counter {
    int id;
    char name[MAX_NAME_LEN];
} counter_t;

typedef struct counter_grp {
    int id;
    char name[MAX_NAME_LEN];
    const char *description;
    int size;
    const counter_t counters[MAX_COUNTER_GRP_SIZE];
} counter_grp_t;

enum {
    COUNTER_GRP_IPC,
    NUMBER_OF_COUNTER_GRPS,
};

const counter_t *get_counter(int counter_id);

const counter_grp_t *get_counter_grp(int ctr_grp_id);

#endif
