#ifndef CYCLOPS_H
#define CYCLOPS_H

#include <stdbool.h>

#define MAX_BATCH_RUNS 100
#define MAX_PERF_COUNTERS 15
#define MAX_PERF_RATIOS 15

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

    METRIC_RDTSCP,
    METRIC_ARM,

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

} metric_id_t;

typedef enum {
    METRIC_TYPE_RAW,
    METRIC_TYPE_DERIVED,
} metric_type_t;

typedef enum {
    METRIC_BE_PERF,
    METRIC_BE_CPU_INSTRUCTION,
    N_METRIC_BACKENDS,
} metric_backend_id_t;

typedef struct {
    metric_id_t id;
    const char *name;
    metric_type_t type;
    metric_backend_id_t backend;
    metric_id_t numerator;
    metric_id_t denominator;
    /* TODO: add supported architectures */
} metric_t;

typedef struct {
    bool display_help;
    unsigned long long warmup_runs;
    unsigned long long batch_runs;
    char *wl_name;
    char *mg_name;
    int n_wl_params;
    char **wl_param_keys;
    char **wl_param_args;
    char *ps_wl_param_key;
    char *ps_wl_param_low;
    char *ps_wl_param_high;
    char *ps_wl_param_step;
    bool batch_csv;
    bool param_sweep_csv;
    bool report;
} cyclops_cfg_t;

#endif
