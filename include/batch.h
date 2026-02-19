#ifndef BATCH_H
#define BATCH_H

#include <stdint.h>

#include "./microbench.h"
#include "./metric.h"

typedef struct batch_metrics {
    int warmup_runs;
    int batch_runs;
    metric_grp_t metric_grp;
    counter_metric_t counters[MAX_COUNTER_GRP_SIZE];
    ratio_metric_t ratios[MAX_RATIO_METRICS];
} batch_metrics_t;

batch_metrics_t init_batch_metrics(int warmup_runs, int batch_runs, metric_grp_id_t id);

void process_batch_metrics(batch_metrics_t *batch_metrics);

void run_batch(int workload_id, int ctr_grp_id);

#endif
