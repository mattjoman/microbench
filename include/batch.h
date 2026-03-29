#ifndef BATCH_H
#define BATCH_H

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"
#include "./data_processing.h"

typedef struct batch_conf {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    const metric_grp_t *mg;
} batch_conf_t;

typedef struct {
    uint64_t *run_vals;
    uint64_agg_t agg;
} perf_time_data_t;

typedef struct {
    const metric_t *metric;
    double *run_vals;
    double_agg_t agg;
} perf_counter_data_t;

typedef struct {
    const metric_t *metric;
    double *run_vals;
    double_agg_t agg;
} perf_ratio_data_t;

typedef struct {

    perf_time_data_t time_enabled;
    perf_time_data_t time_running;

    int n_perf_counters;
    perf_counter_data_t *perf_counters;

    int n_perf_ratios;
    perf_ratio_data_t *perf_ratios;
} perf_batch_t;

/*** TIMER ***/

typedef struct {
    uint64_t *run_vals;
    uint64_agg_t agg;
} timer_data_t;

typedef struct {
    timer_data_t timer;
} timer_batch_t;

void run_batch(unsigned long long warmup_runs,
               unsigned long long batch_runs,
               workload_t *wl,
               const metric_grp_t *mg);

#endif
