#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_L1D_READ_ACCESSES,
    METRIC_L1D_READ_MISSES,
    METRIC_L1D_READ_MISS_RATE,
};

static metric_grp_t mg = {
    .name = "L1D_READS",
    .backend = METRIC_BE_PERF,
    .n_metrics = 4,
    .metrics = metrics,
};

REGISTER_MG(&mg)
