#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "./cyclops.h"
#include "./metric_grp.h"
#include "./workload.h"

///////////////////////////////////////////////////////////////////////////////
/// BATCH

typedef struct double_agg {
    double min;
    double max;
    double median;
} double_agg_t;

typedef struct metric_grp metric_grp_t;

typedef struct {
    metric_id_t metric_id;
    double *run_vals;
    double_agg_t agg;
} metric_data_t;

typedef struct batch {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    metric_grp_t *mg;

    metric_data_t raw_data_scaling;

    int n_raw;
    metric_data_t *raw_data;

    int n_derived;
    metric_data_t *derived_data;

    bool to_csv;
} batch_t;

void batch_single_run(cyclops_cfg_t *cyclops_cfg);

///////////////////////////////////////////////////////////////////////////////
/// PARAM SWEEP

typedef struct {
    unsigned long long param_sweep_val;
    double_agg_t agg;
} ps_batch_data_t;

typedef struct {
    metric_id_t metric_id;
    ps_batch_data_t *batch_vals;
} ps_data_t;

typedef struct {

    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    metric_grp_t *mg;

    char *wl_param_key;
    char *wl_param_low;
    char *wl_param_high;
    char *wl_param_step;

    unsigned long long n_batches;
    ps_data_t *data;

    bool to_csv;
} param_sweep_t;

void param_sweep_run(cyclops_cfg_t *cyclops_cfg);

#endif
