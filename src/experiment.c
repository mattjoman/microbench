#include <string.h>
#include <stdlib.h>

#include "../include/experiment.h"
#include "../include/counter.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/analysis.h"
#include "../include/report.h"

static batch_t batch_init(int warmup_runs, int batch_runs,
                                           ctr_grp_t ctr_grp)
{
    if (batch_runs < 1 || batch_runs > MAX_BATCH_SIZE)
        abort();

    if (ctr_grp.size < 1 || ctr_grp.size > MAX_COUNTER_GRP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;
    batch.ctr_grp_size = ctr_grp.size;

    memcpy(batch.ctr_grp, ctr_grp.ctr_ids,
                              ctr_grp.size * sizeof(int));

    return batch;
}

void run_experiment(int workload_id, int ctr_grp_id)
{
    batch_t batch;
    workload_t workload;
    ctr_grp_t ctr_grp;
    analysis_t analysis;

    int batch_runs = MAX_BATCH_SIZE;
    int warmup_runs = 5;

    ctr_grp = *get_ctr_grp(ctr_grp_id);

    batch = batch_init(warmup_runs, batch_runs, ctr_grp);

    workload = *get_workload(workload_id);

    workload.init();
    bench_perf_event(&batch, workload.workload);
    workload.clean();

    analysis = run_analysis(&batch, ctr_grp);

    run_report(ctr_grp, analysis);
}
