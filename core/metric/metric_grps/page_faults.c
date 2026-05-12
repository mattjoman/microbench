#include "../../../include/metric.h"

static metric_id_t metrics[] = {
    METRIC_PAGE_FAULTS,
    METRIC_PAGE_FAULTS_MAJ,
    METRIC_PAGE_FAULTS_MIN,
};

static metric_grp_t mg = {
    .name = "PAGE_FAULTS",
    .backend = METRIC_BE_PERF,
    .n_metrics = 3,
    .metrics = metrics,
};

REGISTER_MG(&mg)
