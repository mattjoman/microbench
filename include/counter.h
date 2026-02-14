#ifndef EVENT_H
#define EVENT_H

#define MAX_CTR_GRP_SIZE 3

typedef struct ctr_grp {
    int id;
    int size;
    int ctr_ids[MAX_CTR_GRP_SIZE];
} ctr_grp_t;

enum {
    CTR_CPU_CYCLES,
    CTR_REF_CPU_CYCLES,
    CTR_INSTRUCTIONS,
    CTR_CACHE_ACCESSES,
    CTR_CACHE_MISSES,
    CTR_L1_CACHE_MISSES,
    CTR_BRANCH_INSTRUCTIONS,
    CTR_BRANCH_MISPREDICTIONS,
    CTR_PAGE_FAULTS,
    CTR_CPU_CLOCK_NS,
    CTR_TASK_CLOCK_NS,
    CTR_ALIGNMENT_FAULTS,
    NUMBER_OF_CTRS,
};

extern const char *metric_names[NUMBER_OF_CTRS];

enum {
    CTR_GRP_IPC,
    NUMBER_OF_CTR_GRPS,
};

extern const char *ctr_grp_names[NUMBER_OF_CTR_GRPS];

const ctr_grp_t *get_ctr_grp(int ctr_grp_id);

#endif
