#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/experiment.h"
#include "../../include/metric_grp.h"
#include "../../include/workload.h"
#include "./internal.h"

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

static param_sweep_t *param_sweep_init(cyclops_cfg_t *cyclops_cfg)
{
    param_sweep_t *ps = NULL;
    if (!(ps = calloc(1, sizeof(param_sweep_t)))) {
        perror("Failed to allocate memory for param_sweep_t struct");
        exit(1);
    }

    metric_grp_t *mg = mg_get_by_name(cyclops_cfg->mg_name);
    workload_t *wl = wl_get_by_name(cyclops_cfg->wl_name);

    for (int i = 0; i < cyclops_cfg->n_wl_params; i++) {
        wl_set_param(wl,
                     cyclops_cfg->wl_param_keys[i],
                     cyclops_cfg->wl_param_args[i]);
    }

    ps->wl = wl;
    ps->mg = mg;
    ps->warmup_runs = cyclops_cfg->warmup_runs;
    ps->batch_runs = cyclops_cfg->batch_runs;
    ps->wl_param_key = cyclops_cfg->ps_wl_param_key;
    ps->wl_param_low = cyclops_cfg->ps_wl_param_low;
    ps->wl_param_high = cyclops_cfg->ps_wl_param_high;
    ps->wl_param_step = cyclops_cfg->ps_wl_param_step;
    ps->n_batches = ps_n_batches(ps);
    ps->to_csv = cyclops_cfg->param_sweep_csv;

    if (!(ps->data = calloc(mg->n_metrics, sizeof(ps_data_t)))) {
        perror("Failed to allocate memory for ps_data_t array");
        exit(1);
    }

    for (int i = 0; i < mg->n_metrics; i++) {
        ps->data[i].batch_vals = calloc(ps->n_batches,
                                        sizeof(ps_batch_data_t));
        if (!ps->data[i].batch_vals) {
            perror("Failed to allocate memory for ps_batch_data array");
            exit(1);
        }

        ps->data[i].metric_id = mg->metrics[i];
    }

    return ps;
}

static void param_sweep_destroy(param_sweep_t *ps)
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

void param_sweep_run(cyclops_cfg_t *cyclops_cfg)
{
    param_sweep_t *ps = param_sweep_init(cyclops_cfg);

    static char param_val_buf[64];
    unsigned long long param_val;

    for (unsigned long long i = 0; i < ps->n_batches; i++) {

        batch_t *batch_data = batch_init(cyclops_cfg);

        /* 
         * must set param to current sweep value after calling
         * batch_init(), not before
         */
        param_val = ps_get_nth_param_val(ps, i);
        snprintf(param_val_buf, sizeof(param_val_buf), "%llu", param_val);
        wl_set_param(ps->wl, ps->wl_param_key, param_val_buf);

        batch_param_sweep_run(batch_data, i);

        /* extract aggregate batch data for each metric */
        for (int m = 0; m < ps->mg->n_metrics; m++) {
            metric_data_t *batch_metric_data = batch_get_metric_data(
                                            batch_data, ps->data[m].metric_id);
            assert(batch_metric_data);
            ps->data[m].batch_vals[i].agg = batch_metric_data->agg;
            ps->data[m].batch_vals[i].param_sweep_val = param_val;
        }

        destroy_batch_data(batch_data);
    }

    /* write to csv */
    param_sweep_to_csv(ps);

    /* clean up */
    param_sweep_destroy(ps);
}
