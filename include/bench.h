#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"
#include "./metric.h"

uint64_t bench_rdtscp(void (*test_func)(void));

int bench_perf_event(batch_metrics_t *batch_metrics, void (*workload)(void));

#endif
