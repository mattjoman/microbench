#include <string.h>
#include <stdlib.h>

#include "../include/batch.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/data_processing.h"
#include "../include/report.h"

void run_batch(int workload_id, int ctr_grp_id)
{
    batch_metrics_t batch_metrics;

    int batch_runs = MAX_BATCH_SIZE;
    int warmup_runs = 5;

    workload_t workload = *get_workload(workload_id);

    batch_metrics = init_batch_metrics(warmup_runs, batch_runs, ctr_grp_id);

    workload.init();
    bench_perf_event(&batch_metrics, workload.workload);
    workload.clean();

    process_batch_metrics(&batch_metrics);

    run_report(batch_metrics);
}
