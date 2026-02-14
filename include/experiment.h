#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <stdint.h>

#include "./event.h"

#define MAX_BENCH_BATCH_SIZE 100

typedef struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    int ctr_grp_size; // actual number of metrics
    int ctr_grp[MAX_CTR_GRP_SIZE];
    uint64_t results[NUMBER_OF_CTRS][MAX_BENCH_BATCH_SIZE];
} batch_t;

void run_experiment(int workload_id, int ctr_grp_id);

#endif
