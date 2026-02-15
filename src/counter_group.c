#include <stdlib.h>

#include "../include/counter_group.h"

const counter_t counter_cpu_cycles = {
    .id = COUNTER_CPU_CYCLES,
    .name = "COUNTER_CPU_CYCLES",
};

const counter_t counter_ref_cpu_cycles = {
    .id = COUNTER_REF_CPU_CYCLES,
    .name = "COUNTER_REF_CPU_CYCLES",
};

const counter_t counter_instructions = {
    .id = COUNTER_INSTRUCTIONS,
    .name = "COUNTER_INSTRUCTIONS",
};

const counter_t *get_counter(int counter_id)
{
    switch (counter_id) {

        case COUNTER_CPU_CYCLES:
            return &counter_cpu_cycles;

        case COUNTER_REF_CPU_CYCLES:
            return &counter_ref_cpu_cycles;

        case COUNTER_INSTRUCTIONS:
            return &counter_instructions;

        default:
            break;
    }

    abort();

    return NULL;
}

static counter_grp_t ctr_grp_ipc = {
    .id = COUNTER_GRP_IPC,
    .name = "COUNTER_GRP_IPC",
    .size = MAX_COUNTER_GRP_SIZE,
    .counters = {
        counter_cpu_cycles,
        counter_ref_cpu_cycles,
        counter_instructions,
    },
};

const counter_grp_t *get_counter_grp(int ctr_grp_id)
{
    switch (ctr_grp_id) {

        case COUNTER_GRP_IPC:
            return &ctr_grp_ipc;

        default:
            break;
    }

    abort();

    return NULL;
}
