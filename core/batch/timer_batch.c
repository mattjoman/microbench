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

static timer_batch_t *init_timer_batch_data(batch_conf_t *cfg)
{
    timer_batch_t *data = calloc(1, sizeof(timer_batch_t));
    if (!data) {
        perror("Failed to allocate memory for batch data struct");
        exit(1);
    }

    data->timer.run_vals = alloc_uint64_array(cfg->batch_runs);

    return data;
}

static void destroy_timer_batch_data(timer_batch_t *batch_data)
{
    free(batch_data->timer.run_vals);
    batch_data->timer.run_vals = NULL;

    free(batch_data);
    batch_data = NULL;
}

static void process_timer_batch(batch_conf_t *cfg,
                                timer_batch_t *batch_data)
{
    batch_data->timer.agg = aggregate_uint64(batch_data->timer.run_vals,
                                             cfg->batch_runs);
}

void run_timer_batch(batch_conf_t *cfg)
{
    workload_t *wl = cfg->wl;
    timer_batch_t *batch_data = init_timer_batch_data(cfg);
    bench_func_t bench_func = get_timer_bench_func(cfg->mg->id);

    wl->init(wl);
    bench_func(cfg, batch_data, wl->workload);
    wl->clean();

    process_timer_batch(cfg, batch_data);

    timer_batch_to_csv(cfg, batch_data);
    run_timer_report(cfg, batch_data);

    destroy_timer_batch_data(batch_data);
}
