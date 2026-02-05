#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

enum _metrics {
    METRIC_CPU_CYCLES,
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

typedef struct metric_agg metric_agg_t;
struct metric_agg {
    uint64_t min;
    uint64_t max;
    uint64_t median;
};

metric_agg_t metric_agg(uint64_t batch_metric_results[], int batch_runs);

extern const char *metric_names[NUMBER_OF_METRICS];

#define MAX_EVENT_GROUP_SIZE 3
#define MAX_BENCH_BATCH_SIZE 100

typedef struct batch batch_t;

struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    int event_group_size; // actual number of metrics
    int event_group[MAX_EVENT_GROUP_SIZE];
    uint64_t results[NUMBER_OF_METRICS][MAX_BENCH_BATCH_SIZE];
};

enum _workloads {
    WL_CONTIGUOUS_ARRAY,
    WL_SCATTERED_ARRAY,
};

typedef struct workload {
    void (*init)(void);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

/*** ====================== WORKLOADS ====================== ***/

const workload_t *get_workload(int workload);

/*** ====================== BENCHMARKS ====================== ***/

uint64_t bench_rdtscp(void (*test_func)(void));
int bench_perf_event(batch_t *batch, void (*test_func)(void));

void experiment_1(void);

#endif
