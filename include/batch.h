#ifndef BATCH_H
#define BATCH_H

#include <stdint.h>

#include "./microbench.h"
#include "./metric.h"
#include "./workload.h"

typedef struct batch_conf {
    int warmup_runs;
    int batch_runs;
    int workload_id;
    metric_grp_id_t metric_grp_id;
} batch_conf_t;

typedef struct batch_data {
    metric_id_t metric_id_map[NUMBER_OF_METRICS];
    counter_metric_t counters[MAX_COUNTER_GRP_SIZE];
    ratio_metric_t ratios[MAX_RATIO_METRICS];
} batch_data_t;

void run_batch(batch_conf_t batch_config);

#endif
