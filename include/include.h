#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>

enum metric {
    METRIC_CPU_CYCLES
    ,METRIC_L1_CACHE_MISSES
    ,METRIC_BRANCH_MISPREDICTIONS
    ,METRIC_PAGE_FAULTS
    ,NUMBER_OF_METRICS
};

static const unsigned int METRICS[NUMBER_OF_METRICS] = {
    METRIC_CPU_CYCLES
    ,METRIC_L1_CACHE_MISSES
    ,METRIC_BRANCH_MISPREDICTIONS
    ,METRIC_PAGE_FAULTS
};

struct benchmark_results {
    uint64_t values[NUMBER_OF_METRICS];
};

void test_loop();
void test_contiguous_array();
void test_scattered_array();

uint64_t bench_rdtscp(void (*test_func)(void));
struct benchmark_results bench_1(void (*test_func)(void));

void run_rdtscp_test_loop();
void run_test_cache_miss();
void run_bench_1();

#endif

