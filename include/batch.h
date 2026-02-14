#ifndef BATCH_H
#define BATCH_H

#include <stdint.h>

#include "./counter.h"

#define MAX_BATCH_SIZE 100

typedef struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    int ctr_grp_size; // actual number of counters
    int ctr_grp[MAX_COUNTER_GRP_SIZE];
    uint64_t results[NUMBER_OF_COUNTERS][MAX_BATCH_SIZE];
} batch_t;

void run_batch(int workload_id, int ctr_grp_id);

#endif
