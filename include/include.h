#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_EVENT_GROUP_SIZE 3
#define MAX_BENCH_BATCH_SIZE 100

enum {
    METRIC_CPU_CYCLES,
    METRIC_REF_CPU_CYCLES,
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

extern const char *metric_names[NUMBER_OF_METRICS];

enum {
    EVENT_GROUP_IPC,
    //EVENT_GROUP_FEND_VS_BEND,
    //EVENT_GROUP_BRANCH_EFFICIENCY,
    NUMBER_OF_EVENT_GROUPS,
};

extern const char *event_group_names[NUMBER_OF_EVENT_GROUPS];

enum {
    WL_CONTIGUOUS_ARRAY,
    WL_SCATTERED_ARRAY,
    NUMBER_OF_WORKLOADS,
};

extern const char *workload_names[NUMBER_OF_WORKLOADS];

typedef struct metric_agg {
    int event_id;
    uint64_t min;
    uint64_t max;
    uint64_t median;
} metric_agg_t;

typedef struct ratio_agg {
    double min;
    double max;
    double median;
} ratio_agg_t;

typedef struct analysis {
    metric_agg_t event_aggs[MAX_EVENT_GROUP_SIZE];
    ratio_agg_t ratio_agg;
} analysis_t;

typedef struct event_group {
    int id;
    int size;
    int event_ids[MAX_EVENT_GROUP_SIZE];
} event_group_t;

typedef struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    int event_group_size; // actual number of metrics
    int event_group[MAX_EVENT_GROUP_SIZE];
    uint64_t results[NUMBER_OF_METRICS][MAX_BENCH_BATCH_SIZE];
} batch_t;

typedef struct workload {
    void (*init)(void);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

/*** ====================== WORKLOADS ====================== ***/

const workload_t *get_workload(int workload_id);
const event_group_t *get_event_group(int event_group_id);

/*** ====================== BENCHMARKS ====================== ***/

uint64_t bench_rdtscp(void (*test_func)(void));
int bench_perf_event(batch_t *batch, void (*workload)(void));

void run_experiment(int workload_id, int egroup_id);

void run_report(event_group_t egroup, analysis_t analysis);

analysis_t run_analysis(batch_t *batch, event_group_t egroup);

#endif
