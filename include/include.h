#ifndef BENCH_H
#define BENCH_H

#include <stdint.h>
#include <stdlib.h>

struct benchmark_results {
  uint64_t cpu_cycles;
  uint64_t l1_cache_misses;
  uint64_t branch_mispredictions;
  uint64_t page_faults;
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

