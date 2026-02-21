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
    counter_id_t counter_id_map[NUMBER_OF_COUNTERS];
    counter_metric_t counters[MAX_COUNTER_GRP_SIZE];
    ratio_metric_t ratios[MAX_RATIO_METRICS];
} batch_data_t;

int init_batch_conf(batch_conf_t *batch_conf, int warmup_runs,
                                              int batch_runs,
                                              int workload_id,
                                              metric_grp_id_t id);

void run_batch(batch_conf_t batch_config);

#endif
