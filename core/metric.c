#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/metric.h"
#include "../include/data_processing.h"

const perf_counter_metric_t perf_counters[N_PERF_COUNTERS] = {
    [PC_ID_CPU_CYCLES] = {
        .id = PC_ID_CPU_CYCLES,
        .name = "CPU_CYCLES",
    },
    [PC_ID_REF_CPU_CYCLES] = {
        .id = PC_ID_REF_CPU_CYCLES,
        .name = "REF_CPU_CYCLES",
    },
    [PC_ID_INSTRUCTIONS] = {
        .id = PC_ID_INSTRUCTIONS,
        .name = "INSTRUCTIONS",
    },
    [PC_ID_LLC_READ_ACCESSES] = {
        .id = PC_ID_LLC_READ_ACCESSES,
        .name = "LLC_READ_ACCESSES",
    },
    [PC_ID_LLC_READ_MISSES] = {
        .id = PC_ID_LLC_READ_MISSES,
        .name = "LLC_READ_MISSES",
    },
    [PC_ID_L1D_READ_ACCESSES] = {
        .id = PC_ID_L1D_READ_ACCESSES,
        .name = "L1D_READ_ACCESSES",
    },
    [PC_ID_L1D_READ_MISSES] = {
        .id = PC_ID_L1D_READ_MISSES,
        .name = "L1D_READ_MISSES",
    },
    [PC_ID_L1I_READ_ACCESSES] = {
        .id = PC_ID_L1I_READ_ACCESSES,
        .name = "L1I_READ_ACCESSES",
    },
    [PC_ID_L1I_READ_MISSES] = {
        .id = PC_ID_L1I_READ_MISSES,
        .name = "L1I_READ_MISSES",
    },
    [PC_ID_DTLB_READ_ACCESSES] = {
        .id = PC_ID_DTLB_READ_ACCESSES,
        .name = "DTLB_READ_ACCESSES",
    },
    [PC_ID_DTLB_READ_MISSES] = {
        .id = PC_ID_DTLB_READ_MISSES,
        .name = "DTLB_READ_MISSES",
    },
    [PC_ID_ITLB_READ_ACCESSES] = {
        .id = PC_ID_ITLB_READ_ACCESSES,
        .name = "ITLB_READ_ACCESSES",
    },
    [PC_ID_ITLB_READ_MISSES] = {
        .id = PC_ID_ITLB_READ_MISSES,
        .name = "ITLB_READ_MISSES",
    },
    [PC_ID_BPU_READ_ACCESSES] = {
        .id = PC_ID_BPU_READ_ACCESSES,
        .name = "BPU_READ_ACCESSES",
    },
    [PC_ID_BPU_READ_MISSES] = {
        .id = PC_ID_BPU_READ_MISSES,
        .name = "BPU_READ_MISSES",
    },
    [PC_ID_BRANCH_INSTRUCTIONS] = {
        .id = PC_ID_BRANCH_INSTRUCTIONS,
        .name = "BRANCH_INSTRUCTIONS",
    },
    [PC_ID_BRANCH_MISPREDICTIONS] = {
        .id = PC_ID_BRANCH_MISPREDICTIONS,
        .name = "BRANCH_MISPREDICTIONS",
    },
    [PC_ID_STALLED_CYCLES_FRONTEND] = {
        .id = PC_ID_STALLED_CYCLES_FRONTEND,
        .name = "STALLED_CYCLES_FRONTEND",
    },
    [PC_ID_STALLED_CYCLES_BACKEND]  = {
        .id = PC_ID_STALLED_CYCLES_BACKEND,
        .name = "STALLED_CYCLES_BACKEND",
    },
    [PC_ID_PAGE_FAULTS] = {
        .id = PC_ID_PAGE_FAULTS,
        .name = "PAGE_FAULTS",
    },
    [PC_ID_PAGE_FAULTS_MAJ] = {
        .id = PC_ID_PAGE_FAULTS_MAJ,
        .name = "PAGE_FAULTS_MAJ",
    },
    [PC_ID_PAGE_FAULTS_MIN] = {
        .id = PC_ID_PAGE_FAULTS_MIN,
        .name = "PAGE_FAULTS_MIN",
    },
    [PC_ID_CPU_CLOCK_NS] = {
        .id = PC_ID_CPU_CLOCK_NS,
        .name = "CPU_CLOCK_NS",
    },
    [PC_ID_TASK_CLOCK_NS] = {
        .id = PC_ID_TASK_CLOCK_NS,
        .name = "TASK_CLOCK_NS",
    },
    [PC_ID_ALIGNMENT_FAULTS] = {
        .id = PC_ID_ALIGNMENT_FAULTS,
        .name = "ALIGNMENT_FAULTS",
    },
};

const perf_ratio_metric_t perf_ratios[N_PERF_RATIOS] = {
    [PR_ID_INSTRUCTIONS_PER_CYCLE] = {
        .id = PR_ID_INSTRUCTIONS_PER_CYCLE,
        .name = "IPC",
        .numerator_id = PC_ID_INSTRUCTIONS,
        .denominator_id = PC_ID_CPU_CYCLES,
    },
    [PR_ID_CYCLES_PER_INSTRUCTION] = {
        .id = PR_ID_CYCLES_PER_INSTRUCTION,
        .name = "CPI",
        .numerator_id = PC_ID_CPU_CYCLES,
        .denominator_id = PC_ID_INSTRUCTIONS,
    },
    [PR_ID_LLC_READ_MISS_RATE] = {
        .id = PR_ID_LLC_READ_MISS_RATE,
        .name = "LLC_READ_MISS_RATE",
        .numerator_id = PC_ID_LLC_READ_MISSES,
        .denominator_id = PC_ID_LLC_READ_ACCESSES,
    },
    [PR_ID_L1D_READ_MISS_RATE] = {
        .id = PR_ID_L1D_READ_MISS_RATE,
        .name = "L1D_READ_MISS_RATE",
        .numerator_id = PC_ID_L1D_READ_MISSES,
        .denominator_id = PC_ID_L1D_READ_ACCESSES,
    },
    [PR_ID_L1I_READ_MISS_RATE] = {
        .id = PR_ID_L1I_READ_MISS_RATE,
        .name = "L1I_READ_MISS_RATE",
        .numerator_id = PC_ID_L1I_READ_MISSES,
        .denominator_id = PC_ID_L1I_READ_ACCESSES,
    },
    [PR_ID_DTLB_READ_MISS_RATE] = {
        .id = PR_ID_DTLB_READ_MISS_RATE,
        .name = "DTLB_READ_MISS_RATE",
        .numerator_id = PC_ID_DTLB_READ_MISSES,
        .denominator_id = PC_ID_DTLB_READ_ACCESSES,
    },
    [PR_ID_ITLB_READ_MISS_RATE] = {
        .id = PR_ID_ITLB_READ_MISS_RATE,
        .name = "ITLB_READ_MISS_RATE",
        .numerator_id = PC_ID_ITLB_READ_MISSES,
        .denominator_id = PC_ID_ITLB_READ_ACCESSES,
    },
    [PR_ID_BPU_READ_MISS_RATE] = {
        .id = PR_ID_BPU_READ_MISS_RATE,
        .name = "BPU_READ_MISS_RATE",
        .numerator_id = PC_ID_BPU_READ_MISSES,
        .denominator_id = PC_ID_BPU_READ_ACCESSES,
    },
    [PR_ID_BRANCH_MISPRED_RATE] = {
        .id = PR_ID_BRANCH_MISPRED_RATE,
        .name = "BRANCH_MISPRED_RATE",
        .numerator_id = PC_ID_BRANCH_MISPREDICTIONS,
        .denominator_id = PC_ID_BRANCH_INSTRUCTIONS,
    },
    [PR_ID_FE_VS_BE_STALLS] = {
        .id = PR_ID_FE_VS_BE_STALLS,
        .name = "FE_VS_BE_STALLS",
        .numerator_id = PC_ID_STALLED_CYCLES_FRONTEND,
        .denominator_id = PC_ID_STALLED_CYCLES_BACKEND,
    },
};

const metric_grp_t metric_grps[] = {

    {
        .name = "TEST",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            NULL,
        },
    },

    {
        .name = "BIG",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_REF_CPU_CYCLES],
            &perf_counters[PC_ID_INSTRUCTIONS],
            &perf_counters[PC_ID_LLC_READ_ACCESSES],
            &perf_counters[PC_ID_LLC_READ_MISSES],
            &perf_counters[PC_ID_L1D_READ_ACCESSES],
            &perf_counters[PC_ID_L1D_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_INSTRUCTIONS_PER_CYCLE],
            &perf_ratios[PR_ID_CYCLES_PER_INSTRUCTION],
            &perf_ratios[PR_ID_LLC_READ_MISS_RATE],
            &perf_ratios[PR_ID_L1D_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "IPC",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_REF_CPU_CYCLES],
            &perf_counters[PC_ID_INSTRUCTIONS],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_INSTRUCTIONS_PER_CYCLE],
            &perf_ratios[PR_ID_CYCLES_PER_INSTRUCTION],
            NULL,
        },
    },

    {
        .name = "LLC_READS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_LLC_READ_ACCESSES],
            &perf_counters[PC_ID_LLC_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_LLC_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "L1D_READS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_L1D_READ_ACCESSES],
            &perf_counters[PC_ID_L1D_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_L1D_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "L1I_READS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_L1I_READ_ACCESSES],
            &perf_counters[PC_ID_L1I_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_L1I_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "DTLB_READS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_DTLB_READ_ACCESSES],
            &perf_counters[PC_ID_DTLB_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_DTLB_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "ITLB_READS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_ITLB_READ_ACCESSES],
            &perf_counters[PC_ID_ITLB_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_ITLB_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "BPU_READS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_BPU_READ_ACCESSES],
            &perf_counters[PC_ID_BPU_READ_MISSES],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_BPU_READ_MISS_RATE],
            NULL,
        },
    },

    {
        .name = "BRANCH",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_BRANCH_INSTRUCTIONS],
            &perf_counters[PC_ID_BRANCH_MISPREDICTIONS],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_BRANCH_MISPRED_RATE],
            NULL,
        },
    },

    {
        .name = "STALLED_CYCLES",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_STALLED_CYCLES_FRONTEND],
            &perf_counters[PC_ID_STALLED_CYCLES_BACKEND],
            NULL,
        },
        .perf_ratios = (const perf_ratio_metric_t *const[]){
            &perf_ratios[PR_ID_FE_VS_BE_STALLS],
            NULL,
        },
    },

    {
        .name = "PAGE_FAULTS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_PAGE_FAULTS],
            &perf_counters[PC_ID_PAGE_FAULTS_MAJ],
            &perf_counters[PC_ID_PAGE_FAULTS_MIN],
            NULL,
        },
    },

    {
        .name = "ALIGNMENT_FAULTS",
        .type = MG_TYPE_PERF,
        .perf_counters = (const perf_counter_metric_t *const[]){
            &perf_counters[PC_ID_CPU_CYCLES],
            &perf_counters[PC_ID_ALIGNMENT_FAULTS],
            NULL,
        },
    },

    {
        .name = "RDTSCP",
        .type = MG_TYPE_TIMER,
    },

    { 0 }, // end of mg array

};

int mg_n_perf_counters(const metric_grp_t *mg)
{
    int n = 0;
    const perf_counter_metric_t *const *counter = mg->perf_counters;
    while (counter && *counter) {
        n++;
        counter++;
    }
    return n;
}

int mg_n_perf_ratios(const metric_grp_t *mg)
{
    int n = 0;
    const perf_ratio_metric_t *const *ratio = mg->perf_ratios;
    while (ratio && *ratio) {
        n++;
        ratio++;
    }
    return n;
}

const metric_grp_t *get_mg_by_name(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    const metric_grp_t *mg = &metric_grps[0];
    while (mg->name) {
        if (strcmp(name, mg->name) == 0) {
            return mg;
        }
        mg++;
    }

    fprintf(stderr, "No metric group with name '%s'\n", name);
    return NULL;
}

void print_metric_grp_guide(void)
{
    printf("Metric groups:\n\n");

    const metric_grp_t *mg = &metric_grps[0];
    while (mg->name) {
        printf("  %s:\n", mg->name);

        if (mg->type != MG_TYPE_PERF) {
            printf("\n");
            mg++;
            continue;
        }

        printf("    Raw:  ");
        const perf_counter_metric_t *const *perf_counter = mg->perf_counters;
        while (perf_counter != NULL && *perf_counter != NULL) {
            printf("%s  ", (*perf_counter)->name);
            perf_counter++;
        }
        printf("\n");

        printf("    Derived:  ");
        const perf_ratio_metric_t *const *perf_ratio = mg->perf_ratios;
        while (perf_ratio != NULL && *perf_ratio != NULL) {
            printf("%s  ", (*perf_ratio)->name);
            perf_ratio ++;
        }
        printf("\n\n");

        mg++;
    }
}
