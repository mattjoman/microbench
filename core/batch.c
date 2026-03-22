#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/batch.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

int init_batch_conf(batch_conf_t *batch_conf, unsigned long long warmup_runs,
                                              unsigned long long batch_runs,
                                              workload_t *wl,
                                              const metric_grp_t *mg)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_RUNS) {
        batch_runs = 100;
    }

    batch_conf->warmup_runs   = warmup_runs;
    batch_conf->batch_runs    = batch_runs;
    batch_conf->wl            = wl;
    batch_conf->mg            = mg;

    return 0;
}

static uint64_t *alloc_uint64_array(unsigned long long length)
{
    uint64_t *array = calloc(length, sizeof(uint64_t));
    if (!array) {
        perror("Failed to allocate uint64 array");
        exit(1);
    }
    return array;
}

static double *alloc_double_array(unsigned long long length)
{
    double *array = calloc(length, sizeof(double));
    if (!array) {
        perror("Failed to allocate double array");
        exit(1);
    }
    return array;
}

static batch_data_t *init_perf_batch_data(batch_conf_t batch_cfg)
{
    batch_data_t *data;
    if (!(data = calloc(1, sizeof(batch_data_t)))) {
        perror("Failed to allocate memory for batch_data_t struct");
        exit(1);
    }

    data->time_enabled.run_vals = alloc_uint64_array(batch_cfg.batch_runs);
    data->time_running.run_vals = alloc_uint64_array(batch_cfg.batch_runs);

    const metric_t *counter_metric_buff[MAX_PERF_COUNTERS];
    const metric_t *ratio_metric_buff[MAX_PERF_RATIOS];

    mg_list_metrics_by_type(batch_cfg.mg, METRIC_TYPE_PERF_COUNTER,
            MAX_PERF_COUNTERS, counter_metric_buff, &data->n_perf_counters);
    mg_list_metrics_by_type(batch_cfg.mg, METRIC_TYPE_PERF_RATIO,
            MAX_PERF_RATIOS, ratio_metric_buff, &data->n_perf_ratios);

    if (!data->n_perf_counters) {
        fprintf(stderr, "No perf counter metrics\n");
        exit(1);
    }

    if (!(data->perf_counters = calloc(data->n_perf_counters,
                                       sizeof(perf_counter_data_t)))) {
        perror("Failed to allocate memory for perf counters");
        exit(1);
    }

    if (data->n_perf_ratios) {
        if (!(data->perf_ratios = calloc(data->n_perf_ratios,
                                         sizeof(perf_ratio_data_t)))) {
            perror("Failed to allocate memory for perf ratios");
            exit(1);
        }
    }

    for (int i = 0; i < data->n_perf_counters; i++) {
        data->perf_counters[i].run_vals = alloc_uint64_array(
                                                        batch_cfg.batch_runs);
        data->perf_counters[i].metric = counter_metric_buff[i];
    }

    for (int i = 0; i < data->n_perf_ratios; i++) {
        data->perf_ratios[i].run_vals = alloc_double_array(
                                                        batch_cfg.batch_runs);
        data->perf_ratios[i].metric = ratio_metric_buff[i];
    }

    return data;
}

static void destroy_perf_batch_data(batch_data_t *batch_data)
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
    free(batch_data->perf_ratios);
    free(batch_data);
    batch_data = NULL;
}

static batch_data_t *init_timer_batch_data(batch_conf_t batch_cfg)
{
    batch_data_t *data = calloc(1, sizeof(batch_data_t));
    if (!data) {
        perror("Failed to allocate memory for batch data struct");
        exit(1);
    }

    data->timer.run_vals = alloc_uint64_array(batch_cfg.batch_runs);
    data->timer.metric = batch_cfg.mg->metrics[0];

    return data;
}

static void destroy_timer_batch_data(batch_data_t *batch_data)
{
    free(batch_data->timer.run_vals);
    batch_data->timer.run_vals = NULL;

    free(batch_data);
    batch_data = NULL;
}

static void process_perf_counter_data(batch_conf_t batch_conf,
                                      batch_data_t *batch_data)
{
    uint64_agg_t agg;
    int batch_runs = batch_conf.batch_runs;

    for (int i = 0; i < batch_data->n_perf_counters; i++) {
        agg = aggregate_uint64(batch_data->perf_counters[i].run_vals,
                                                                batch_runs);
        batch_data->perf_counters[i].agg = agg;
    }
}

static void process_perf_ratio_data(batch_conf_t batch_cfg,
                                    batch_data_t *batch_data)
{
    double_agg_t agg;
    uint64_t *numerators, *denominators;
    metric_id_t numerator_id, denominator_id;

    for (int i = 0; i < batch_data->n_perf_ratios; i++) {

        denominator_id = batch_data->perf_ratios[i].metric->denominator_id;
        numerator_id = batch_data->perf_ratios[i].metric->numerator_id;

        numerators = denominators = NULL;

        for (int j = 0; j < batch_data->n_perf_counters; j++) {
            if (batch_data->perf_counters[j].metric->id == numerator_id) {
                numerators = batch_data->perf_counters[j].run_vals;
                break;
            }
        }

        for (int j = 0; j < batch_data->n_perf_counters; j++) {
            if (batch_data->perf_counters[j].metric->id == denominator_id) {
                denominators = batch_data->perf_counters[j].run_vals;
                break;
            }
        }

        if (!numerators || !denominators) {
            fprintf(stderr, "No numerators or denominators\n");
            exit(1);
        }

        calc_ratios(batch_data->perf_ratios[i].run_vals,
                    numerators,
                    denominators,
                    batch_cfg.batch_runs);

        agg = aggregate_double(batch_data->perf_ratios[i].run_vals,
                                                        batch_cfg.batch_runs);
        batch_data->perf_ratios[i].agg = agg;
    }
}

static void process_timer_batch(batch_conf_t batch_conf,
                         batch_data_t *batch_data)
{
    uint64_agg_t agg;
    int batch_runs = batch_conf.batch_runs;

    agg = aggregate_uint64(batch_data->timer.run_vals, batch_runs);
    batch_data->timer.agg = agg;
}

static void process_perf_batch(batch_conf_t batch_conf,
                              batch_data_t *batch_data)
{
    process_perf_counter_data(batch_conf, batch_data);
    process_perf_ratio_data(batch_conf, batch_data);
}

void run_batch(batch_conf_t batch_conf)
{

    batch_data_t *batch_data;
    workload_t *wl = batch_conf.wl;
    const metric_grp_t *mg = batch_conf.mg;

    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        batch_data = init_timer_batch_data(batch_conf);
    } else {
        batch_data = init_perf_batch_data(batch_conf);
    }

    wl->init(wl);

    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        bench_timer(batch_conf, batch_data, wl->workload);
    } else {
        bench_perf_event_open(batch_conf, batch_data, wl->workload);
    }

    wl->clean();

    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        process_timer_batch(batch_conf, batch_data);
    } else {
        process_perf_batch(batch_conf, batch_data);
    }

    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        timer_batch_to_csv(batch_conf, batch_data);
    } else {
        perf_batch_to_csv(batch_conf, batch_data);
    }

    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        // TODO: implement run_timer_report(batch_conf, batch_data);
    } else {
        run_perf_report(batch_conf, batch_data);
    }


    if (mg->type == METRIC_GRP_TYPE_TIMER) {
        destroy_timer_batch_data(batch_data);
    } else {
        destroy_perf_batch_data(batch_data);
    }
}
