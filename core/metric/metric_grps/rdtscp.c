#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_RDTSCP,
};

static metric_grp_t mg = {
    .name = "RDTSCP",
    .backend = METRIC_BE_CPU_INSTRUCTION,
    .n_metrics = 1,
    .metrics = metrics,
};

REGISTER_MG(&mg)
