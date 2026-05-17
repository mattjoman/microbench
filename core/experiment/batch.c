#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/experiment.h"
#include "../../include/metric_grp.h"
#include "../../include/workload.h"
#include "./internal.h"

static double *alloc_double_array(unsigned long long length)
{
    double *array = calloc(length, sizeof(double));
    if (!array) {
        perror("Failed to allocate double array");
        exit(1);
    }
    return array;
}

batch_t *batch_init(cyclops_cfg_t *cyclops_cfg)
{
    batch_t *b;
    if (!(b = calloc(1, sizeof(batch_t)))) {
        perror("Failed to allocate memory for batch struct");
        exit(1);
    }

    metric_grp_t *mg = mg_get_by_name(cyclops_cfg->mg_name);
    workload_t *wl = wl_get_by_name(cyclops_cfg->wl_name);

    for (int i = 0; i < cyclops_cfg->n_wl_params; i++) {
        wl_set_param(wl,
                     cyclops_cfg->wl_param_keys[i],
                     cyclops_cfg->wl_param_args[i]);
    }

    b->warmup_runs   = cyclops_cfg->warmup_runs;
    b->batch_runs    = cyclops_cfg->batch_runs;
    b->wl            = wl;
    b->mg            = mg;

    b->n_raw = mg_n_raw(mg);
    b->n_derived = mg_n_derived(mg);
    b->to_csv = cyclops_cfg->batch_csv;

    assert(b->n_raw > 0);

    if (!(b->raw_data = calloc(b->n_raw, sizeof(metric_data_t)))) {
        perror("Failed to allocate memory for perf counters");
        exit(1);
    }

    if (b->n_derived) {
        if (!(b->derived_data = calloc(b->n_derived,
                                          sizeof(metric_data_t)))) {
            perror("Failed to allocate memory for perf ratios");
            exit(1);
        }
    }

    b->raw_data_scaling.run_vals = alloc_double_array(b->batch_runs);

    for (int i = 0; i < b->n_raw; i++) {
        b->raw_data[i].run_vals = alloc_double_array(b->batch_runs);
        b->raw_data[i].metric_id = mg_get_nth_raw_id(mg, i);
    }

    for (int i = 0; i < b->n_derived; i++) {
        b->derived_data[i].run_vals = alloc_double_array(b->batch_runs);
        b->derived_data[i].metric_id = mg_get_nth_derived_id(mg, i);
    }

    return b;
}

void destroy_batch_data(batch_t *b)
{
    for (int i = 0; i < b->n_raw; i++) {
        free(b->raw_data[i].run_vals);
        b->raw_data[i].run_vals = NULL;
    }

    for (int i = 0; i < b->n_derived; i++) {
        free(b->derived_data[i].run_vals);
        b->derived_data[i].run_vals = NULL;
    }

    free(b->raw_data_scaling.run_vals);
    b->raw_data_scaling.run_vals = NULL;

    free(b->raw_data);
    b->raw_data = NULL;

    free(b->derived_data);
    b->derived_data = NULL;

    free(b);
    b = NULL;
}

metric_data_t *batch_get_metric_data(batch_t *b,
                                     metric_id_t metric_id)
{
    for (int i = 0; i < b->n_raw; i++) {
        if (b->raw_data[i].metric_id == metric_id) {
            return &b->raw_data[i];
        }
    }
    for (int i = 0; i < b->n_derived; i++) {
        if (b->derived_data[i].metric_id == metric_id) {
            return &b->derived_data[i];
        }
    }
    return NULL;
}

static void batch_process_raw_metric_data(batch_t *b)
{
    b->raw_data_scaling.agg = aggregate_double(
                b->raw_data_scaling.run_vals,
                b->batch_runs);

    for (int i = 0; i < b->n_raw; i++) {
        b->raw_data[i].agg = aggregate_double(
                b->raw_data[i].run_vals,
                b->batch_runs);
    }
}

static void batch_process_derived_metric_data(batch_t *b)
{
    metric_data_t *ratio, *numerator, *denominator;

    for (int i = 0; i < b->n_derived; i++) {

        ratio = &b->derived_data[i];
        const metric_t *m = metric_get_by_id(ratio->metric_id);

        numerator = batch_get_metric_data(b, m->numerator);
        denominator = batch_get_metric_data(b, m->denominator);

        assert(numerator != NULL);
        assert(denominator != NULL);

        calc_ratios(ratio->run_vals,
                    numerator->run_vals,
                    denominator->run_vals,
                    b->batch_runs);

        ratio->agg = aggregate_double(ratio->run_vals, b->batch_runs);
    }
}

static void batch_run(batch_t *b, unsigned long long batch_no)
{
    const metric_backend_t *backend = metric_backend_get(b->mg->backend);

    b->wl->init(b->wl);
    backend->bench_func(b, b->wl->workload);
    b->wl->clean();

    batch_process_raw_metric_data(b);
    batch_process_derived_metric_data(b);

    if (b->to_csv) {
        batch_to_csv(b, batch_no);
    }
    run_report(b);
}

void batch_single_run(cyclops_cfg_t *cyclops_cfg)
{
    batch_t *b = batch_init(cyclops_cfg);
    batch_run(b, 0);
    destroy_batch_data(b);
}

void batch_param_sweep_run(batch_t *b, unsigned long long batch_no)
{
    batch_run(b, batch_no);
}
