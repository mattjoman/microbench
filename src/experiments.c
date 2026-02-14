#include <string.h>
#include <stdlib.h>

#include "../include/experiment.h"
#include "../include/event.h"
#include "../include/bench.h"
#include "../include/workload.h"
#include "../include/analysis.h"
#include "../include/report.h"

static batch_t batch_init(int warmup_runs, int batch_runs,
                                           event_group_t event_group)
{
    if (batch_runs < 1 || batch_runs > MAX_BENCH_BATCH_SIZE)
        abort();

    if (event_group.size < 1 || event_group.size > MAX_EVENT_GROUP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;
    batch.event_group_size = event_group.size;

    memcpy(batch.event_group, event_group.event_ids,
                              event_group.size * sizeof(int));

    return batch;
}

void run_experiment(int workload_id, int egroup_id)
{
    batch_t batch;
    workload_t workload;
    event_group_t event_group;
    analysis_t analysis;

    int batch_runs = MAX_BENCH_BATCH_SIZE;
    int warmup_runs = 5;

    event_group = *get_event_group(egroup_id);

    batch = batch_init(warmup_runs, batch_runs, event_group);

    workload = *get_workload(workload_id);

    workload.init();
    bench_perf_event(&batch, workload.workload);
    workload.clean();

    analysis = run_analysis(&batch, event_group);

    run_report(event_group, analysis);
}
