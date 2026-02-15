#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"
#include "./counter_group.h"

uint64_t bench_rdtscp(void (*test_func)(void));

int bench_perf_event(batch_t *batch, void (*workload)(void),
                                     counter_grp_t ctr_grp);

#endif
