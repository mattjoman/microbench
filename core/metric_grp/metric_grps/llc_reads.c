#include "../../../include/metric_grp.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_LLC_READ_ACCESSES,
    METRIC_LLC_READ_MISSES,
    METRIC_LLC_READ_MISS_RATE,
};

static metric_grp_t mg = {
    .name = "LLC_READS",
    .backend = METRIC_BE_PERF,
    .n_metrics = 4,
    .metrics = metrics,
};

REGISTER_MG(&mg)
