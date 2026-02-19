#include <string.h>
#include <stdlib.h>

#include "../include/batch.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

batch_data_t init_batch_data(int warmup_runs, int batch_runs, metric_grp_id_t id)
{
    metric_grp_t metric_grp = metric_grps[id];

    if (batch_runs < 1 || batch_runs > MAX_BATCH_SIZE)
        abort();

    if (metric_grp.n_counters < 1 || metric_grp.n_counters > MAX_COUNTER_GRP_SIZE)
        abort();

    batch_data_t batch_data;

    batch_data.metric_grp = metric_grp;
    batch_data.warmup_runs = warmup_runs;
    batch_data.batch_runs = batch_runs;

    for (int i = 0; i < metric_grp.n_counters; i++) {

        counter_metric_t counter;
        counter.id = metric_grp.counter_ids[i];

        batch_data.counters[i] = counter;
    }

    for (int i = 0; i < metric_grp.n_ratios; i++) {

        ratio_metric_t ratio;
        ratio.id = metric_grp.ratio_ids[i];

        batch_data.ratios[i] = ratio;
    }

    return batch_data;
}

void process_batch_data(batch_data_t *batch_data)
{
    int batch_runs = batch_data->batch_runs;

    metric_id_t id_map[NUMBER_OF_METRICS];

    for (int i = 0; i < batch_data->metric_grp.n_counters; i++) {

        uint64_agg_t c_agg = aggregate_uint64(batch_data->counters[i].raw, batch_runs);

        batch_data->counters[i].min = c_agg.min;
        batch_data->counters[i].max = c_agg.max;
        batch_data->counters[i].median = c_agg.median;

        id_map[batch_data->counters[i].id] = i;
    }

    calc_ratios(batch_data->ratios[0].raw, batch_data->counters[id_map[METRIC_INSTRUCTIONS]].raw,
                                              batch_data->counters[id_map[METRIC_CPU_CYCLES]].raw,
                                              batch_runs);

    double_agg_t r_agg = aggregate_double(batch_data->ratios[0].raw, batch_runs);

    batch_data->ratios[0].min = r_agg.min;
    batch_data->ratios[0].max = r_agg.max;
    batch_data->ratios[0].median = r_agg.median;
}

void run_batch(int workload_id, int ctr_grp_id)
{
    batch_data_t batch_data;

    int batch_runs = MAX_BATCH_SIZE;
    int warmup_runs = 5;

    workload_t workload = *get_workload(workload_id);

    batch_data = init_batch_data(warmup_runs, batch_runs, ctr_grp_id);

    workload.init();
    bench_perf_event(&batch_data, workload.workload);
    workload.clean();

    process_batch_data(&batch_data);

    run_report(batch_data);
}
