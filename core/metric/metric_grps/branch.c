#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_BRANCH_INSTRUCTIONS,
    METRIC_BRANCH_MISPREDICTIONS,
    METRIC_BRANCH_MISPRED_RATE,
};

static metric_grp_t mg = {
    .name = "BRANCH",
    .backend = METRIC_BE_PERF,
    .n_metrics = 4,
    .metrics = metrics,
};

REGISTER_MG(&mg)
