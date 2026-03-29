#ifndef BENCH_H
#define BENCH_H

#include "./batch.h"
#include "./metric.h"

typedef void (*bench_func_t)(
    batch_conf_t *,
    timer_batch_t *,
    void (*)(void)
);

bench_func_t get_timer_bench_func(mg_id_t id);


int bench_perf_event_open(batch_conf_t *batch_cfg,
                          perf_batch_t *batch_data,
                          void (*workload)(void));

#endif
