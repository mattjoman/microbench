#include <string.h>
#include <stdlib.h>

#include "../include/batch.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

batch_conf_t init_batch_conf(int warmup_runs, int batch_runs,int workload_id, metric_grp_id_t id)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_SIZE)
        abort();

    batch_conf_t batch_conf = {
        .warmup_runs   = warmup_runs,
        .batch_runs    = batch_runs,
        .workload_id   = workload_id,
        .metric_grp_id = id,
    };

    return batch_conf;
}

static batch_data_t init_batch_data(batch_conf_t batch_conf)
{
    metric_grp_t metric_grp = metric_grps[batch_conf.metric_grp_id];

    batch_data_t batch_data;

    for (int i = 0; i < metric_grp.n_counters; i++) {

        counter_metric_t counter;
        counter.id = metric_grp.counter_ids[i];

        batch_data.counters[i] = counter;
        batch_data.metric_id_map[metric_grp.counter_ids[i]] = i;
    }

    for (int i = 0; i < metric_grp.n_ratios; i++) {

        ratio_metric_t ratio;
        ratio.id = metric_grp.ratio_ids[i];

        batch_data.ratios[i] = ratio;
    }

    return batch_data;
}

static void process_batch_data(batch_conf_t batch_conf, batch_data_t *batch_data)
{
    metric_grp_id_t metric_grp_id = batch_conf.metric_grp_id;
    int batch_runs = batch_conf.batch_runs;
    int n_counters = metric_grps[metric_grp_id].n_counters;

    for (int i = 0; i < n_counters; i++) {

        uint64_agg_t c_agg = aggregate_uint64(batch_data->counters[i].raw,
                                              batch_runs);

        batch_data->counters[i].agg = c_agg;
    }

    metric_id_t ratio_id = metric_grps[metric_grp_id].ratio_ids[0];
    metric_id_t numerator_id = ratio_confs[ratio_id].numerator_id;
    metric_id_t denominator_id = ratio_confs[ratio_id].denominator_id;

    int numerator_idx = batch_data->metric_id_map[numerator_id];
    int denominator_idx = batch_data->metric_id_map[denominator_id];

    uint64_t *numerators = batch_data->counters[numerator_idx].raw;
    uint64_t *denominators = batch_data->counters[denominator_idx].raw;

    calc_ratios(batch_data->ratios[0].raw, numerators, denominators,
                                                       batch_runs);

    double_agg_t r_agg = aggregate_double(batch_data->ratios[0].raw,
                                          batch_runs);

    batch_data->ratios[0].agg = r_agg;
}

void run_batch(batch_conf_t batch_conf)
{
    batch_data_t batch_data;
    workload_t workload;

    batch_data = init_batch_data(batch_conf);
    workload = *get_workload(batch_conf.workload_id);

    workload.init();
    bench_perf_event(batch_conf, &batch_data, workload.workload);
    workload.clean();

    process_batch_data(batch_conf, &batch_data);

    run_report(batch_conf, batch_data);
}
