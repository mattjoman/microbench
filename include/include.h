#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>

#define BIG_NUMBER_1 1000000

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

static const unsigned int METRICS[NUMBER_OF_METRICS] = {
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
};

struct benchmark_results {
    uint64_t values[NUMBER_OF_METRICS];
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
struct benchmark_results bench_perf_event(void (*test_func)(void), unsigned int warmup_runs);

void run_rdtscp_test_loop();
void run_test_cache_miss();
void run_bench_1();

#endif

