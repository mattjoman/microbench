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
    int batch_runs = batch_conf.batch_runs;
    int n_counters = metric_grps[batch_conf.metric_grp_id].n_counters;

    for (int i = 0; i < n_counters; i++) {

        uint64_agg_t c_agg = aggregate_uint64(batch_data->counters[i].raw, batch_runs);

        batch_data->counters[i].min = c_agg.min;
        batch_data->counters[i].max = c_agg.max;
        batch_data->counters[i].median = c_agg.median;

    }

    calc_ratios(batch_data->ratios[0].raw, batch_data->counters[batch_data->metric_id_map[METRIC_INSTRUCTIONS]].raw,
                                              batch_data->counters[batch_data->metric_id_map[METRIC_CPU_CYCLES]].raw,
                                              batch_runs);

    double_agg_t r_agg = aggregate_double(batch_data->ratios[0].raw, batch_runs);

    batch_data->ratios[0].min = r_agg.min;
    batch_data->ratios[0].max = r_agg.max;
    batch_data->ratios[0].median = r_agg.median;
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
