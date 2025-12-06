#ifndef BENCH_H
#define BENCH_H

#include <stdint.h>
#include <stdlib.h>

void test_loop();
void test_contiguous_array();
void test_scattered_array();

uint64_t bench_rdtscp(void (*f)(void));
uint64_t bench_cache_miss(void (*f)(void));

void run_rdtscp_test_loop();
void run_test_cache_miss();

#endif

