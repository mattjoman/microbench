#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_STALLED_CYCLES_FRONTEND,
    METRIC_STALLED_CYCLES_BACKEND,
    METRIC_FE_VS_BE_STALLS,
};

static metric_grp_t mg = {
    .name = "STALLED_CYCLES",
    .backend = METRIC_BE_PERF,
    .n_metrics = 4,
    .metrics = metrics,
};

REGISTER_MG(&mg)
