#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_ARM,
};

static metric_grp_t mg = {
    .name = "ARM_TIMER",
    .backend = METRIC_BE_CPU_INSTRUCTION,
    .n_metrics = 1,
    .metrics = metrics,
};

REGISTER_MG(&mg)
