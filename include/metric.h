#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"

typedef enum {

    /* counters */

    METRIC_CPU_CYCLES,
    METRIC_REF_CPU_CYCLES,
    METRIC_INSTRUCTIONS,
    METRIC_LLC_READ_ACCESSES,
    METRIC_LLC_READ_MISSES,
    METRIC_L1D_READ_ACCESSES,
    METRIC_L1D_READ_MISSES,
    METRIC_L1I_READ_ACCESSES,
    METRIC_L1I_READ_MISSES,
    METRIC_DTLB_READ_ACCESSES,
    METRIC_DTLB_READ_MISSES,
    METRIC_ITLB_READ_ACCESSES,
    METRIC_ITLB_READ_MISSES,
    METRIC_BPU_READ_ACCESSES,
    METRIC_BPU_READ_MISSES,
    METRIC_BRANCH_INSTRUCTIONS,
    METRIC_BRANCH_MISPREDICTIONS,
    METRIC_STALLED_CYCLES_FRONTEND,
    METRIC_STALLED_CYCLES_BACKEND,
    METRIC_PAGE_FAULTS,
    METRIC_PAGE_FAULTS_MAJ,
    METRIC_PAGE_FAULTS_MIN,
    METRIC_CPU_CLOCK_NS,
    METRIC_TASK_CLOCK_NS,
    METRIC_ALIGNMENT_FAULTS,

    /* ratios */

    METRIC_INSTRUCTIONS_PER_CYCLE,
    METRIC_CYCLES_PER_INSTRUCTION,
    METRIC_LLC_READ_MISS_RATE,
    METRIC_L1D_READ_MISS_RATE,
    METRIC_L1I_READ_MISS_RATE,
    METRIC_DTLB_READ_MISS_RATE,
    METRIC_ITLB_READ_MISS_RATE,
    METRIC_BPU_READ_MISS_RATE,
    METRIC_BRANCH_MISPRED_RATE,
    METRIC_FE_VS_BE_STALLS,

    N_METRICS

} metric_id_t; // TODO: metric_id_t => perf_sub_metric_id_t

enum {
    METRIC_TYPE_PERF_COUNTER,
    METRIC_TYPE_PERF_RATIO,
};

// TODO: metric_t =>  perf_sub_metric_t
typedef struct {
    metric_id_t id;
    const char *name;
    int type;
    metric_id_t numerator_id;
    metric_id_t denominator_id;
} metric_t;

/*** METRIC GROUPS ***/

enum {
    METRIC_GRP_TYPE_PERF,
    METRIC_GRP_TYPE_TIMER,
};

// TODO: metric_grp_t => metric_t
typedef struct {
    /* must be unique */
    const char *name;

    // TODO: change this to id (or just add id), and make it the primary key
    int type;

    /* only populated for perf metric groups */
    const metric_t *const *perf_counters;
    const metric_t *const *perf_ratios;
} metric_grp_t;

const metric_grp_t *get_mg_by_name(const char *name);
int mg_n_perf_counters(const metric_grp_t *mg);
int mg_n_perf_ratios(const metric_grp_t *mg);
void print_metric_grp_guide(void);

#endif
