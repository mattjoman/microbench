#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdint.h>
#include <stdlib.h>

#define METRIC_CPU_CYCLES 0
#define METRIC_L1_CACHE_MISSES 1
#define METRIC_BRANCH_MISPREDICTIONS 2
#define METRIC_PAGE_FAULTS 3

#define METRICS ((int[]) { \
    METRIC_CPU_CYCLES \
    ,METRIC_L1_CACHE_MISSES \
    ,METRIC_BRANCH_MISPREDICTIONS \
    ,METRIC_PAGE_FAULTS \
})

#define NUMBER_OF_METRICS (sizeof(METRICS) / sizeof(int))

struct benchmark_results {
  uint64_t values[NUMBER_OF_METRICS];
};

void test_loop();
void test_contiguous_array();
void test_scattered_array();

uint64_t bench_rdtscp(void (*f)(void));
struct benchmark_results bench_1();

void run_rdtscp_test_loop();
void run_test_cache_miss();
void run_bench_1();

#endif

