#ifndef BENCH_H
#define BENCH_H

#include <stdint.h>

void test_loop();

uint64_t bench_rdtscp(void (*f)(void));

void run_rdtscp_test_loop();

#endif

