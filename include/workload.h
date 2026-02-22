#ifndef WORKLOAD_H
#define WORKLOAD_H

#define BIG_NUMBER_1 10000000

enum {
    WL_CONTIGUOUS_ARRAY,
    WL_SCATTERED_ARRAY,
    NUMBER_OF_WORKLOADS,
};

typedef struct workload {
    int id;
    const char* name;
    void (*init)(void);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

extern workload_t *all_workloads[NUMBER_OF_WORKLOADS];

extern workload_t wl_contiguous_array;
extern workload_t wl_scattered_array;

#endif
