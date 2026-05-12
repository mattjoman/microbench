#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_ALIGNMENT_FAULTS,
};

static metric_grp_t mg = {
    .name = "ALIGNMENT_FAULTS",
    .backend = METRIC_BE_PERF,
    .n_metrics = 2,
    .metrics = metrics,
};

REGISTER_MG(&mg)
