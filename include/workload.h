#ifndef WORKLOAD_H
#define WORKLOAD_H

enum {
    WL_CONTIGUOUS_ARRAY,
    WL_SCATTERED_ARRAY,
    NUMBER_OF_WORKLOADS,
};

extern const char *workload_names[NUMBER_OF_WORKLOADS];

typedef struct workload {
    void (*init)(void);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

const workload_t *get_workload(int workload_id);

#endif
