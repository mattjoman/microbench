#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"

uint64_t bench_rdtscp(void (*test_func)(void));

int bench_perf_event(batch_t *batch, void (*workload)(void));

#endif
