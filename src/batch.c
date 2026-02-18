#include <string.h>
#include <stdlib.h>

#include "../include/batch.h"
#include "../include/counter_group.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

static batch_t init_batch(int warmup_runs, int batch_runs,
                                           counter_grp_t ctr_grp)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_SIZE)
        abort();

    if (ctr_grp.size < 1 || ctr_grp.size > MAX_COUNTER_GRP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;

    return batch;
}

void run_batch(int workload_id, int ctr_grp_id)
{
    batch_t batch;
    batch_metrics_t batch_metrics;

    int batch_runs = MAX_BATCH_SIZE;
    int warmup_runs = 5;

    counter_grp_t ctr_grp = *get_counter_grp(ctr_grp_id);
    workload_t workload = *get_workload(workload_id);

    batch = init_batch(warmup_runs, batch_runs, ctr_grp);

    workload.init();
    bench_perf_event(&batch, workload.workload, ctr_grp);
    workload.clean();

    batch_metrics = init_batch_metrics(&batch, ctr_grp);

    run_report(batch_metrics);
}
