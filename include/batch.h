#ifndef BATCH_H
#define BATCH_H

#include <stdbool.h>

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"
#include "./data_processing.h"

typedef struct metric_grp metric_grp_t;

typedef struct batch_conf {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    const metric_grp_t *mg;
} batch_conf_t;

typedef struct {
    metric_id_t metric_id;
    double *run_vals;
    double_agg_t agg;
} metric_data_t;

typedef struct batch_data {

    metric_data_t raw_data_scaling;

    int n_raw;
    metric_data_t *raw_data;

    int n_derived;
    metric_data_t *derived_data;
} batch_data_t;

/*** PARAM SWEEP *************************************************************/

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
    const metric_grp_t *mg;

    char *wl_param_key;
    char *wl_param_low;
    char *wl_param_high;
    char *wl_param_step;

    char *file_name;

    unsigned long long n_batches;
    ps_data_t *data;
} param_sweep_t;

/*** CYCLOPS CONFIG **********************************************************/

typedef struct {
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    workload_t *wl;
    const metric_grp_t *mg;
    char *ps_wl_param_key;
    char *ps_wl_param_low;
    char *ps_wl_param_high;
    char *ps_wl_param_step;
    char *file_name;
} cyclops_cfg_t;

void run_cyclops(cyclops_cfg_t *cyclops_cfg);

#endif
