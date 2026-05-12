#include "../../../include/metric_grp.h"

static metric_id_t metrics[] = {
    METRIC_CPU_CYCLES,
    METRIC_INSTRUCTIONS,
    METRIC_INSTRUCTIONS_PER_CYCLE,
    METRIC_CYCLES_PER_INSTRUCTION,
};

static metric_grp_t mg = {
    .name = "IPC",
    .backend = METRIC_BE_PERF,
    .n_metrics = 4,
    .metrics = metrics,
};

REGISTER_MG(&mg)
