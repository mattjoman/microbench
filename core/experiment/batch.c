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

batch_t *init_batch_data(cyclops_cfg_t *cyclops_cfg)
{
    batch_t *data;
    if (!(data = calloc(1, sizeof(batch_t)))) {
        perror("Failed to allocate memory for batch struct");
        exit(1);
    }

    metric_grp_t *mg = get_mg_by_name(cyclops_cfg->mg_name);
    workload_t *wl = get_workload_by_name(cyclops_cfg->wl_name);

    for (int i = 0; i < cyclops_cfg->n_wl_params; i++) {
        wl_set_param(wl,
                     cyclops_cfg->wl_param_keys[i],
                     cyclops_cfg->wl_param_args[i]);
    }

    data->warmup_runs   = cyclops_cfg->warmup_runs;
    data->batch_runs    = cyclops_cfg->batch_runs;
    data->wl            = wl;
    data->mg            = mg;

    data->n_raw = mg_n_raw(mg);
    data->n_derived = mg_n_derived(mg);
    data->to_csv = cyclops_cfg->batch_csv;

    assert(data->n_raw > 0);

    if (!(data->raw_data = calloc(data->n_raw, sizeof(metric_data_t)))) {
        perror("Failed to allocate memory for perf counters");
        exit(1);
    }

    if (data->n_derived) {
        if (!(data->derived_data = calloc(data->n_derived,
                                          sizeof(metric_data_t)))) {
            perror("Failed to allocate memory for perf ratios");
            exit(1);
        }
    }

    data->raw_data_scaling.run_vals = alloc_double_array(data->batch_runs);

    for (int i = 0; i < data->n_raw; i++) {
        data->raw_data[i].run_vals = alloc_double_array(data->batch_runs);
        data->raw_data[i].metric_id = mg_get_nth_raw_id(mg, i);
    }

    for (int i = 0; i < data->n_derived; i++) {
        data->derived_data[i].run_vals = alloc_double_array(data->batch_runs);
        data->derived_data[i].metric_id = mg_get_nth_derived_id(mg, i);
    }

    return data;
}

void destroy_batch_data(batch_t *data)
{
    for (int i = 0; i < data->n_raw; i++) {
        free(data->raw_data[i].run_vals);
        data->raw_data[i].run_vals = NULL;
    }

    for (int i = 0; i < data->n_derived; i++) {
        free(data->derived_data[i].run_vals);
        data->derived_data[i].run_vals = NULL;
    }

    free(data->raw_data_scaling.run_vals);
    data->raw_data_scaling.run_vals = NULL;

    free(data->raw_data);
    data->raw_data = NULL;

    free(data->derived_data);
    data->derived_data = NULL;

    free(data);
    data = NULL;
}

metric_data_t *batch_get_metric_data(batch_t *data,
                                     metric_id_t metric_id)
{
    for (int i = 0; i < data->n_raw; i++) {
        if (data->raw_data[i].metric_id == metric_id) {
            return &data->raw_data[i];
        }
    }
    for (int i = 0; i < data->n_derived; i++) {
        if (data->derived_data[i].metric_id == metric_id) {
            return &data->derived_data[i];
        }
    }
    return NULL;
}

static void batch_process_raw_metric_data(batch_t *batch_data)
{
    batch_data->raw_data_scaling.agg = aggregate_double(
                batch_data->raw_data_scaling.run_vals,
                batch_data->batch_runs);

    for (int i = 0; i < batch_data->n_raw; i++) {
        batch_data->raw_data[i].agg = aggregate_double(
                batch_data->raw_data[i].run_vals,
                batch_data->batch_runs);
    }
}

static void batch_process_derived_metric_data(batch_t *b)
{
    metric_data_t *ratio, *numerator, *denominator;

    for (int i = 0; i < b->n_derived; i++) {

        ratio = &b->derived_data[i];
        const metric_t *m = get_metric_by_id(ratio->metric_id);

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

void run_batch(batch_t *batch_data,
               unsigned long long batch_no)
{
    const metric_backend_t *backend = get_backend(batch_data->mg->backend);

    batch_data->wl->init(batch_data->wl);
    backend->bench_func(batch_data, batch_data->wl->workload);
    batch_data->wl->clean();

    batch_process_raw_metric_data(batch_data);
    batch_process_derived_metric_data(batch_data);

    if (batch_data->to_csv) {
        batch_to_csv(batch_data, batch_no);
    }
    run_report(batch_data);
}

void batch_single_run(cyclops_cfg_t *cyclops_cfg)
{
    batch_t *batch = init_batch_data(cyclops_cfg);
    run_batch(batch, 0);
    destroy_batch_data(batch);
}
