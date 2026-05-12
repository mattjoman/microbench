#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/metric_grp.h"
#include "../../include/data_processing.h" // TODO: remove this?

const metric_t metrics[N_METRICS] = {

    /* RAW METRICS */

    [METRIC_CPU_CYCLES] = {
        .name = "CPU_CYCLES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_REF_CPU_CYCLES] = {
        .name = "REF_CPU_CYCLES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_INSTRUCTIONS] = {
        .name = "INSTRUCTIONS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_LLC_READ_ACCESSES] = {
        .name = "LLC_READ_ACCESSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_LLC_READ_MISSES] = {
        .name = "LLC_READ_MISSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_L1D_READ_ACCESSES] = {
        .name = "L1D_READ_ACCESSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_L1D_READ_MISSES] = {
        .name = "L1D_READ_MISSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_L1I_READ_ACCESSES] = {
        .name = "L1I_READ_ACCESSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_L1I_READ_MISSES] = {
        .name = "L1I_READ_MISSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_DTLB_READ_ACCESSES] = {
        .name = "DTLB_READ_ACCESSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_DTLB_READ_MISSES] = {
        .name = "DTLB_READ_MISSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_ITLB_READ_ACCESSES] = {
        .name = "ITLB_READ_ACCESSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_ITLB_READ_MISSES] = {
        .name = "ITLB_READ_MISSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_BPU_READ_ACCESSES] = {
        .name = "BPU_READ_ACCESSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_BPU_READ_MISSES] = {
        .name = "BPU_READ_MISSES",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_BRANCH_INSTRUCTIONS] = {
        .name = "BRANCH_INSTRUCTIONS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_BRANCH_MISPREDICTIONS] = {
        .name = "BRANCH_MISPREDICTIONS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_STALLED_CYCLES_FRONTEND] = {
        .name = "STALLED_CYCLES_FRONTEND",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_STALLED_CYCLES_BACKEND]  = {
        .name = "STALLED_CYCLES_BACKEND",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_PAGE_FAULTS] = {
        .name = "PAGE_FAULTS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_PAGE_FAULTS_MAJ] = {
        .name = "PAGE_FAULTS_MAJ",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_PAGE_FAULTS_MIN] = {
        .name = "PAGE_FAULTS_MIN",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_CPU_CLOCK_NS] = {
        .name = "CPU_CLOCK_NS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_TASK_CLOCK_NS] = {
        .name = "TASK_CLOCK_NS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_ALIGNMENT_FAULTS] = {
        .name = "ALIGNMENT_FAULTS",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_PERF,
    },
    [METRIC_RDTSCP] = {
        .name = "RDTSCP",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_CPU_INSTRUCTION,
    },
    [METRIC_ARM] = {
        .name = "ARM",
        .type = METRIC_TYPE_RAW,
        .backend = METRIC_BE_CPU_INSTRUCTION,
    },

    /* DERIVED METRICS */

    [METRIC_INSTRUCTIONS_PER_CYCLE] = {
        .name = "IPC",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_INSTRUCTIONS,
        .denominator = METRIC_CPU_CYCLES,
    },
    [METRIC_CYCLES_PER_INSTRUCTION] = {
        .name = "CPI",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_CPU_CYCLES,
        .denominator = METRIC_INSTRUCTIONS,
    },
    [METRIC_LLC_READ_MISS_RATE] = {
        .name = "LLC_READ_MISS_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_LLC_READ_MISSES,
        .denominator = METRIC_LLC_READ_ACCESSES,
    },
    [METRIC_L1D_READ_MISS_RATE] = {
        .name = "L1D_READ_MISS_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_L1D_READ_MISSES,
        .denominator = METRIC_L1D_READ_ACCESSES,
    },
    [METRIC_L1I_READ_MISS_RATE] = {
        .name = "L1I_READ_MISS_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_L1I_READ_MISSES,
        .denominator = METRIC_L1I_READ_ACCESSES,
    },
    [METRIC_DTLB_READ_MISS_RATE] = {
        .name = "DTLB_READ_MISS_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_DTLB_READ_MISSES,
        .denominator = METRIC_DTLB_READ_ACCESSES,
    },
    [METRIC_ITLB_READ_MISS_RATE] = {
        .name = "ITLB_READ_MISS_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_ITLB_READ_MISSES,
        .denominator = METRIC_ITLB_READ_ACCESSES,
    },
    [METRIC_BPU_READ_MISS_RATE] = {
        .name = "BPU_READ_MISS_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_BPU_READ_MISSES,
        .denominator = METRIC_BPU_READ_ACCESSES,
    },
    [METRIC_BRANCH_MISPRED_RATE] = {
        .name = "BRANCH_MISPRED_RATE",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_BRANCH_MISPREDICTIONS,
        .denominator = METRIC_BRANCH_INSTRUCTIONS,
    },
    [METRIC_FE_VS_BE_STALLS] = {
        .name = "FE_VS_BE_STALLS",
        .type = METRIC_TYPE_DERIVED,
        .backend = METRIC_BE_PERF,
        .numerator = METRIC_STALLED_CYCLES_FRONTEND,
        .denominator = METRIC_STALLED_CYCLES_BACKEND,
    },
};

static metric_grp_t **metric_grps = NULL;
static size_t n_metric_grps = 0;

void register_metric_grp(metric_grp_t *mg)
{
    metric_grps = realloc(metric_grps,
                          (n_metric_grps + 1) * sizeof(*metric_grps));
    metric_grps[n_metric_grps++] = mg;
}

const metric_t *get_metric_by_id(metric_id_t id)
{
    return &metrics[id];
}

int mg_n_raw(metric_grp_t *mg)
{
    int raw_counter = 0;
    for (int i = 0; i < mg->n_metrics; i++) {
        const metric_t *m = get_metric_by_id(mg->metrics[i]);
        if (m->type == METRIC_TYPE_RAW) {
            raw_counter++;
        }
    }
    return raw_counter;
}

int mg_n_derived(metric_grp_t *mg)
{
    int derived_counter = 0;
    for (int i = 0; i < mg->n_metrics; i++) {
        const metric_t *m = get_metric_by_id(mg->metrics[i]);
        if (m->type == METRIC_TYPE_DERIVED) {
            derived_counter++;
        }
    }
    return derived_counter;
}

metric_id_t mg_get_nth_raw_id(metric_grp_t *mg, int n)
{
    int raw_counter = 0;
    for (int i = 0; i < mg->n_metrics; i++) {

        const metric_t *m = get_metric_by_id(mg->metrics[i]);

        if (m->type == METRIC_TYPE_RAW) {
            if (raw_counter == n) {
                return mg->metrics[i];
            } else {
                raw_counter++;
            }
        }
    }
    fprintf(stderr, "Couldn't find nth raw metric id\n");
    exit(1);
}

metric_id_t mg_get_nth_derived_id(metric_grp_t *mg, int n)
{
    int derived_counter = 0;
    for (int i = 0; i < mg->n_metrics; i++) {

        const metric_t *m = get_metric_by_id(mg->metrics[i]);

        if (m->type == METRIC_TYPE_DERIVED) {
            if (derived_counter == n) {
                return mg->metrics[i];
            } else {
                derived_counter++;
            }
        }
    }
    fprintf(stderr, "Couldn't find nth derived metric id\n");
    exit(1);
}

metric_grp_t *get_mg_by_name(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < n_metric_grps; i++) {
        metric_grp_t *mg = metric_grps[i];
        if (strcmp(name, mg->name) == 0) {
            return mg;
        }
    }

    fprintf(stderr, "No metric group with name '%s'\n", name);
    return NULL;
}

void print_metric_grp_guide(void)
{
    printf("Metric groups:\n\n");

    for (size_t i = 0; i < n_metric_grps; i++) {
        metric_grp_t *mg = metric_grps[i];
        printf("  %s\n", mg->name);
    }
}
