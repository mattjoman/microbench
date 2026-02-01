#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define BIG_NUMBER_1 1000000

enum metric {
    METRIC_CPU_CYCLES
    ,METRIC_INSTRUCTIONS
    //,METRIC_STALLED_CYCLES_FRONTEND
    //,METRIC_STALLED_CYCLES_BACKEND
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

static const uint64_t METRICS[NUMBER_OF_METRICS] = {
    METRIC_CPU_CYCLES
    ,METRIC_INSTRUCTIONS
    //,METRIC_STALLED_CYCLES_FRONTEND
    //,METRIC_STALLED_CYCLES_BACKEND
    ,METRIC_CACHE_ACCESSES
    ,METRIC_CACHE_MISSES
    ,METRIC_L1_CACHE_MISSES
    ,METRIC_BRANCH_INSTRUCTIONS
    ,METRIC_BRANCH_MISPREDICTIONS
    ,METRIC_PAGE_FAULTS
    ,METRIC_CPU_CLOCK_NS
    ,METRIC_TASK_CLOCK_NS
    ,METRIC_ALIGNMENT_FAULTS
};

struct benchmark_results {
    uint64_t values[NUMBER_OF_METRICS];
};


#define MAX_EVENT_GROUP_SIZE 3
#define MAX_BENCH_BATCH_SIZE 1

// matches kernel abi
struct bench_run_results {
    uint64_t nr;
    uint64_t time_enabled;
    uint64_t time_running;
    struct {
        uint64_t value;
        uint64_t id;
    } values[MAX_EVENT_GROUP_SIZE];
};

// for use in my program
struct bench_batch_results {
    char name[64];
    uint64_t runs; // actual runs
    uint64_t event_group_size; // how many metrics
    struct {
        unsigned int id; // e.g. METRIC_CPU_CYCLES
        char name[64];
        uint64_t values[MAX_BENCH_BATCH_SIZE];
    } metrics[MAX_EVENT_GROUP_SIZE];
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
int bench_perf_event(struct bench_batch_results *batch_results, void (*test_func)(void), unsigned int warmup_runs);

void run_rdtscp_test_loop();
void run_test_cache_miss();
void run_bench_1();

#endif

