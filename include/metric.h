#ifndef METRIC_H
#define METRIC_H

#include <stdint.h>

#include "./microbench.h"

/*** PERF ***/

typedef enum {

    /* counters */

    PC_ID_CPU_CYCLES,
    PC_ID_REF_CPU_CYCLES,
    PC_ID_INSTRUCTIONS,
    PC_ID_LLC_READ_ACCESSES,
    PC_ID_LLC_READ_MISSES,
    PC_ID_L1D_READ_ACCESSES,
    PC_ID_L1D_READ_MISSES,
    PC_ID_L1I_READ_ACCESSES,
    PC_ID_L1I_READ_MISSES,
    PC_ID_DTLB_READ_ACCESSES,
    PC_ID_DTLB_READ_MISSES,
    PC_ID_ITLB_READ_ACCESSES,
    PC_ID_ITLB_READ_MISSES,
    PC_ID_BPU_READ_ACCESSES,
    PC_ID_BPU_READ_MISSES,
    PC_ID_BRANCH_INSTRUCTIONS,
    PC_ID_BRANCH_MISPREDICTIONS,
    PC_ID_STALLED_CYCLES_FRONTEND,
    PC_ID_STALLED_CYCLES_BACKEND,
    PC_ID_PAGE_FAULTS,
    PC_ID_PAGE_FAULTS_MAJ,
    PC_ID_PAGE_FAULTS_MIN,
    PC_ID_CPU_CLOCK_NS,
    PC_ID_TASK_CLOCK_NS,
    PC_ID_ALIGNMENT_FAULTS,

    N_PERF_COUNTERS

} perf_counter_id_t;

typedef enum {

    /* ratios */

    PR_ID_INSTRUCTIONS_PER_CYCLE,
    PR_ID_CYCLES_PER_INSTRUCTION,
    PR_ID_LLC_READ_MISS_RATE,
    PR_ID_L1D_READ_MISS_RATE,
    PR_ID_L1I_READ_MISS_RATE,
    PR_ID_DTLB_READ_MISS_RATE,
    PR_ID_ITLB_READ_MISS_RATE,
    PR_ID_BPU_READ_MISS_RATE,
    PR_ID_BRANCH_MISPRED_RATE,
    PR_ID_FE_VS_BE_STALLS,

    N_PERF_RATIOS

} perf_ratio_id_t;

typedef struct {
    perf_counter_id_t id;
    const char *name;
} perf_counter_metric_t;

typedef struct {
    perf_ratio_id_t id;
    const char *name;
    perf_counter_id_t numerator_id;
    perf_counter_id_t denominator_id;
} perf_ratio_metric_t;

/*** METRIC GROUPS ***/

typedef enum {
    MG_TYPE_PERF,
    MG_TYPE_TIMER,
} mg_type_t;

typedef enum {
    MG_ID_RDTSCP,
    MG_ID_ARM_TIMER,
} mg_id_t;

//typedef enum {
//} mg_id_t;

typedef struct {
    /* must be unique */
    const char *name;

    mg_type_t type;
    mg_id_t id;


    /* only populated for perf metric groups */
    const perf_counter_metric_t *const *perf_counters;
    const perf_ratio_metric_t *const *perf_ratios;
} metric_grp_t;

const metric_grp_t *get_mg_by_name(const char *name);
int mg_n_perf_counters(const metric_grp_t *mg);
int mg_n_perf_ratios(const metric_grp_t *mg);
void print_metric_grp_guide(void);

#endif
