#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../../include/batch.h"
#include "../../include/bench.h"
#include "../../include/workload.h"
#include "../../include/data_processing.h"
#include "../../include/report.h"

#include "./internal.h"

static perf_batch_t *init_perf_batch_data(batch_conf_t *cfg)
{
    perf_batch_t *data;
    if (!(data = calloc(1, sizeof(perf_batch_t)))) {
        perror("Failed to allocate memory for perf_batch_t struct");
        exit(1);
    }

    data->time_enabled.run_vals = alloc_uint64_array(cfg->batch_runs);
    data->time_running.run_vals = alloc_uint64_array(cfg->batch_runs);

    const metric_grp_t *mg = cfg->mg;

    data->n_perf_counters = mg_n_perf_counters(mg);
    assert(data->n_perf_counters > 0);

    if (!(data->perf_counters = calloc(data->n_perf_counters,
                                       sizeof(perf_counter_data_t)))) {
        perror("Failed to allocate memory for perf counters");
        exit(1);
    }

    data->n_perf_ratios = mg_n_perf_ratios(mg);
    if (data->n_perf_ratios) {
        if (!(data->perf_ratios = calloc(data->n_perf_ratios,
                                         sizeof(perf_ratio_data_t)))) {
            perror("Failed to allocate memory for perf ratios");
            exit(1);
        }
    }

    for (int i = 0; i < data->n_perf_counters; i++) {
        data->perf_counters[i].run_vals = alloc_double_array(cfg->batch_runs);
        data->perf_counters[i].metric = mg->perf_counters[i];
    }

    for (int i = 0; i < data->n_perf_ratios; i++) {
        data->perf_ratios[i].run_vals = alloc_double_array(cfg->batch_runs);
        data->perf_ratios[i].metric = mg->perf_ratios[i];
    }

    return data;
}

static void destroy_perf_batch_data(perf_batch_t *batch_data)
{
    free(batch_data->time_enabled.run_vals);
    batch_data->time_enabled.run_vals = NULL;

    free(batch_data->time_running.run_vals);
    batch_data->time_running.run_vals = NULL;

    for (int i = 0; i < batch_data->n_perf_counters; i++) {
        free(batch_data->perf_counters[i].run_vals);
        batch_data->perf_counters[i].run_vals = NULL;
    }

    for (int i = 0; i < batch_data->n_perf_ratios; i++) {
        free(batch_data->perf_ratios[i].run_vals);
        batch_data->perf_ratios[i].run_vals = NULL;
    }

    free(batch_data->perf_counters);
    batch_data->perf_counters = NULL;

    free(batch_data->perf_ratios);
    batch_data->perf_ratios = NULL;

    free(batch_data);
    batch_data = NULL;
}

static void process_perf_counter_data(batch_conf_t *cfg,
                                      perf_batch_t *batch_data)
{
    double_agg_t agg;

    for (int i = 0; i < batch_data->n_perf_counters; i++) {
        agg = aggregate_double(batch_data->perf_counters[i].run_vals,
                               cfg->batch_runs);
        batch_data->perf_counters[i].agg = agg;
    }
}

static void process_perf_ratio_data(batch_conf_t *cfg,
                                    perf_batch_t *batch_data)
{
    for (int i = 0; i < batch_data->n_perf_ratios; i++) {

        perf_ratio_data_t *ratio = &batch_data->perf_ratios[i];

        double *numerators = NULL;
        double *denominators = NULL;

        /*
         * Find the numerator and denominator counters needed to calculate the
         * ratio.
         *
         * TODO: switch to index array lookup
         */
        for (int j = 0; j < batch_data->n_perf_counters; j++) {
            if (numerators && denominators) {
                break;
            }

            perf_counter_data_t *counter = &batch_data->perf_counters[j];

            metric_id_t counter_mid = counter->metric->id;
            metric_id_t ratio_numerator_mid = ratio->metric->numerator_id;
            metric_id_t ratio_denominator_mid = ratio->metric->denominator_id;

            if (counter_mid == ratio_numerator_mid) {
                numerators = counter->run_vals;
            } else if (counter_mid == ratio_denominator_mid) {
                denominators = counter->run_vals;
            }
        }

        if (!numerators || !denominators) {
            fprintf(stderr, "No numerators or denominators\n");
            exit(1);
        }

        calc_ratios(batch_data->perf_ratios[i].run_vals,
                    numerators,
                    denominators,
                    cfg->batch_runs);

        ratio->agg = aggregate_double(ratio->run_vals, cfg->batch_runs);
    }
}

void run_perf_batch(batch_conf_t *cfg)
{

    perf_batch_t *batch_data;
    workload_t *wl = cfg->wl;

    batch_data = init_perf_batch_data(cfg);

    wl->init(wl);
    bench_perf_event_open(cfg, batch_data, wl->workload);
    wl->clean();

    process_perf_counter_data(cfg, batch_data);
    process_perf_ratio_data(cfg, batch_data);

    perf_batch_to_csv(cfg, batch_data);
    run_perf_report(cfg, batch_data);

    destroy_perf_batch_data(batch_data);
}
