#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/batch.h"
#include "../include/metric.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

static double *alloc_double_array(unsigned long long length)
{
    double *array = calloc(length, sizeof(double));
    if (!array) {
        perror("Failed to allocate double array");
        exit(1);
    }
    return array;
}

static batch_data_t *init_batch_data(cyclops_cfg_t *cyclops_cfg)
{
    batch_data_t *data;
    if (!(data = calloc(1, sizeof(batch_data_t)))) {
        perror("Failed to allocate memory for batch_data_t struct");
        exit(1);
    }

    data->warmup_runs   = cyclops_cfg->warmup_runs;
    data->batch_runs    = cyclops_cfg->batch_runs;
    data->wl            = cyclops_cfg->wl;
    data->mg            = cyclops_cfg->mg;

    const metric_grp_t *mg = cyclops_cfg->mg;

    data->n_raw = mg_n_raw(mg);
    data->n_derived = mg_n_derived(mg);

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

static void destroy_batch_data(batch_data_t *data)
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

static void process_perf_counter_data(batch_data_t *batch_data)
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

static void process_perf_ratio_data(batch_data_t *bd)
{
    for (int i = 0; i < bd->n_derived; i++) {

        metric_data_t *ratio = &bd->derived_data[i];
        const metric_t *m = get_metric_by_id(ratio->metric_id);

        double *numerators = NULL;
        double *denominators = NULL;

        for (int j = 0; j < bd->n_raw; j++) {
            if (numerators && denominators) {
                break;
            }

            if (bd->raw_data[j].metric_id == m->numerator) {
                numerators = bd->raw_data[j].run_vals;
            } else if (bd->raw_data[j].metric_id == m->denominator) {
                denominators = bd->raw_data[j].run_vals;
            }
        }

        calc_ratios(bd->derived_data[i].run_vals,
                    numerators,
                    denominators,
                    bd->batch_runs);

        ratio->agg = aggregate_double(ratio->run_vals, bd->batch_runs);
    }
}

static void run_batch(batch_data_t *batch_data,
                      bool write_batch_to_csv,
                      unsigned long long batch_no)
{
    const metric_backend_t *backend = get_backend(batch_data->mg->backend);

    batch_data->wl->init(batch_data->wl);
    backend->bench_func(batch_data, batch_data->wl->workload);
    batch_data->wl->clean();

    process_perf_counter_data(batch_data);
    process_perf_ratio_data(batch_data);

    if (write_batch_to_csv) {
        batch_to_csv(batch_data, batch_no);
    }
    run_report(batch_data);
}

static metric_data_t *batch_get_metric_data(batch_data_t *data,
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

/*** PARAM SWEEP *************************************************************/

/*
 * Calculates the number of batches needed to perform the parameter sweep.
 */
static unsigned long long ps_n_batches(param_sweep_t *ps)
{
    unsigned long long low = strtoull(ps->wl_param_low, NULL, 10);
    unsigned long long high = strtoull(ps->wl_param_high, NULL, 10);
    unsigned long long step = strtoull(ps->wl_param_step, NULL, 10);

    unsigned long long diff = high - low;
    unsigned long long mod = diff % step;

    if (mod != 0) {
        return (diff / step) + 2;
    }
    return (diff / step) + 1;
}

static param_sweep_t *init_param_sweep(cyclops_cfg_t *cyclops_cfg)
{
    param_sweep_t *ps = NULL;
    if (!(ps = calloc(1, sizeof(param_sweep_t)))) {
        perror("Failed to allocate memory for param_sweep_t struct");
        exit(1);
    }

    ps->wl = cyclops_cfg->wl;
    ps->mg = cyclops_cfg->mg;

    ps->wl_param_key = cyclops_cfg->ps_wl_param_key;
    ps->wl_param_low = cyclops_cfg->ps_wl_param_low;
    ps->wl_param_high = cyclops_cfg->ps_wl_param_high;
    ps->wl_param_step = cyclops_cfg->ps_wl_param_step;

    ps->file_name = cyclops_cfg->file_name;

    ps->n_batches = ps_n_batches(ps);

    const metric_grp_t *mg = cyclops_cfg->mg;

    if (!(ps->data = calloc(mg->n_metrics, sizeof(ps_data_t)))) {
        perror("Failed to allocate memory for ps_data_t array");
        exit(1);
    }

    for (int i = 0; i < mg->n_metrics; i++) {
        ps->data[i].batch_vals = calloc(ps->n_batches,
                                        sizeof(ps_batch_data_t));
        if (!ps->data[i].batch_vals) {
            perror("Failed to allocate memory for ps_batch_data_t array");
            exit(1);
        }

        ps->data[i].metric_id = mg->metrics[i];
    }

    return ps;
}

static void destroy_param_sweep(param_sweep_t *ps)
{
    for (int i = 0; i < ps->mg->n_metrics; i++) {
        free(ps->data[i].batch_vals);
        ps->data[i].batch_vals = NULL;
    }

    free(ps->data);
    ps->data = NULL;
}

/* n is zero-based */
static unsigned long long ps_get_nth_param_val(param_sweep_t *ps,
                                               unsigned long long n)
{
    unsigned long long low = strtoull(ps->wl_param_low, NULL, 10);
    unsigned long long high = strtoull(ps->wl_param_high, NULL, 10);
    unsigned long long step = strtoull(ps->wl_param_step, NULL, 10);

    if (n == ps->n_batches) {
        return high;
    }
    return low + (n * step);
}

static void run_param_sweep(param_sweep_t *ps, cyclops_cfg_t *cyclops_cfg)
{
    static char param_val_buf[64];

    for (unsigned long long i = 0; i < ps->n_batches; i++) {

        unsigned long long param_val = ps_get_nth_param_val(ps, i);
        snprintf(param_val_buf, sizeof(param_val_buf), "%llu", param_val);

        wl_reset_param(ps->wl, ps->wl_param_key, param_val_buf);

        /* init batch */
        batch_data_t *batch_data = init_batch_data(cyclops_cfg);

        /* run batch */
        run_batch(batch_data, false, i);

        /* extract aggregate batch data for each metric */
        for (int m = 0; m < ps->mg->n_metrics; m++) {
            metric_data_t *batch_metric_data = batch_get_metric_data(
                                            batch_data, ps->data[m].metric_id);
            assert(batch_metric_data);
            ps->data[m].batch_vals[i].agg = batch_metric_data->agg;
            ps->data[m].batch_vals[i].param_sweep_val = param_val;
        }

        /* destroy batch */
        destroy_batch_data(batch_data);
    }

    /* write to csv */
}

void run_cyclops(cyclops_cfg_t *cyclops_cfg)
{
    if (cyclops_cfg->ps_wl_param_key) {
        /* multi-batch param sweep */
        param_sweep_t *ps = init_param_sweep(cyclops_cfg);
        run_param_sweep(ps, cyclops_cfg);
        param_sweep_to_csv(ps);
        destroy_param_sweep(ps);
    } else {
        /* single batch */
        batch_data_t *batch_data = init_batch_data(cyclops_cfg);
        run_batch(batch_data, false, 0);
        destroy_batch_data(batch_data);
    }
}
