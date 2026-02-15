#ifndef BATCH_H
#define BATCH_H

#include <stdint.h>

#include "./counter_group.h"

#define MAX_BATCH_SIZE 100

typedef struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    uint64_t results[NUMBER_OF_COUNTERS][MAX_BATCH_SIZE];
} batch_t;

void run_batch(int workload_id, int ctr_grp_id);

#endif
