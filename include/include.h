#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define BIG_NUMBER_1 10000

enum metric {
    METRIC_CPU_CYCLES
    ,METRIC_INSTRUCTIONS
    ,METRIC_CACHE_ACCESSES
    ,METRIC_CACHE_MISSES
    ,METRIC_L1_CACHE_MISSES
    ,METRIC_BRANCH_INSTRUCTIONS
    ,METRIC_BRANCH_MISPREDICTIONS
    ,METRIC_PAGE_FAULTS
    ,METRIC_CPU_CLOCK_NS
    ,METRIC_TASK_CLOCK_NS
    ,METRIC_ALIGNMENT_FAULTS
    ,NUMBER_OF_METRICS
};

#define MAX_EVENT_GROUP_SIZE 3
#define MAX_BENCH_BATCH_SIZE 1

typedef struct batch batch_t;

struct batch {
    int warmup_runs;
    int batch_runs; // actual runs
    int event_group_size; // actual number of metrics
    int event_group[MAX_EVENT_GROUP_SIZE];
    uint64_t results[NUMBER_OF_METRICS][MAX_BENCH_BATCH_SIZE];
};

/*** ====================== TESTS ====================== ***/

void test_loop();

void init_contiguous_array();
void test_contiguous_array();
void clean_contiguous_array();

void init_scattered_array();
void clean_scattered_array();
void test_scattered_array();

/*** ====================== BENCHMARKS ====================== ***/

uint64_t bench_rdtscp(void (*test_func)(void));
int bench_perf_event(batch_t *batch, void (*test_func)(void));

void run_rdtscp_test_loop();
void run_test_cache_miss();
void run_bench_1();

#endif

