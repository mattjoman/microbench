#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <stdint.h>

#include "./event.h"

#define MAX_BENCH_BATCH_SIZE 100

typedef struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    int event_group_size; // actual number of metrics
    int event_group[MAX_EVENT_GROUP_SIZE];
    uint64_t results[NUMBER_OF_METRICS][MAX_BENCH_BATCH_SIZE];
} batch_t;

void run_experiment(int workload_id, int egroup_id);

#endif
