#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/batch.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

int init_batch_conf(batch_conf_t *batch_conf, int warmup_runs,
                                                            int batch_runs,
                                                            int workload_id,
                                                            metric_grp_id_t id)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_SIZE)
        return -1;

    batch_conf->warmup_runs   = warmup_runs;
    batch_conf->batch_runs    = batch_runs;
    batch_conf->workload_id   = workload_id;
    batch_conf->metric_grp_id = id;

    return 0;
}

static int init_batch_data(batch_data_t *batch_data, batch_conf_t batch_conf)
{
    metric_grp_t metric_grp;

    metric_grp = metric_grps[batch_conf.metric_grp_id];

    for (int i = 0; i < metric_grp.n_counters; i++) {

        counter_metric_t counter;
        counter.id = metric_grp.counter_ids[i];

        batch_data->counters[i] = counter;
        batch_data->metric_id_map[metric_grp.counter_ids[i]] = i;
    }

    for (int i = 0; i < metric_grp.n_ratios; i++) {

        ratio_metric_t ratio;
        ratio.id = metric_grp.ratio_ids[i];

        batch_data->ratios[i] = ratio;
    }

    return 0;
}

static int process_batch_ctrs(batch_conf_t batch_conf,
                               batch_data_t *batch_data)
{
    metric_grp_id_t metric_grp_id;
    uint64_agg_t agg;
    int batch_runs, n_counters;

    batch_runs = batch_conf.batch_runs;
    metric_grp_id = batch_conf.metric_grp_id;
    n_counters = metric_grps[metric_grp_id].n_counters;

    for (int i = 0; i < n_counters; i++) {
        agg = aggregate_uint64(batch_data->counters[i].raw, batch_runs);
        batch_data->counters[i].agg = agg;
    }

    return 0;
}

static int process_batch_ratios(batch_conf_t batch_conf,
                                 batch_data_t *batch_data)
{
    metric_grp_id_t metric_grp_id;
    metric_id_t ratio_id, numerator_id, denominator_id, *metric_id_map;
    double_agg_t agg;
    counter_metric_t *counters;
    ratio_metric_t *ratios;
    int numerator_idx, denominator_idx, batch_runs;
    uint64_t *numerators, *denominators;

    metric_grp_id = batch_conf.metric_grp_id;
    ratio_id = metric_grps[metric_grp_id].ratio_ids[0];
    numerator_id = ratio_confs[ratio_id].numerator_id;
    denominator_id = ratio_confs[ratio_id].denominator_id;

    metric_id_map = batch_data->metric_id_map;

    numerator_idx = metric_id_map[numerator_id];
    denominator_idx = metric_id_map[denominator_id];

    counters = batch_data->counters;
    ratios = batch_data->ratios;

    numerators = counters[numerator_idx].raw;
    denominators = counters[denominator_idx].raw;

    batch_runs = batch_conf.batch_runs;
    calc_ratios(ratios[0].raw, numerators, denominators, batch_runs);

    agg = aggregate_double(ratios[0].raw, batch_runs);
    ratios[0].agg = agg;

    return 0;
}

static void process_batch_data(batch_conf_t batch_conf,
                               batch_data_t *batch_data)
{
    if (process_batch_ctrs(batch_conf, batch_data) != 0)
        exit(1);
    if (process_batch_ratios(batch_conf, batch_data) != 0)
        exit(1);
}

void run_batch(batch_conf_t batch_conf)
{
    batch_data_t batch_data;
    workload_t workload;

    init_batch_data(&batch_data, batch_conf);
    workload = *get_workload(batch_conf.workload_id);

    workload.init();
    bench_perf_event(batch_conf, &batch_data, workload.workload);
    workload.clean();

    process_batch_data(batch_conf, &batch_data);

    run_report(batch_conf, batch_data);
}
