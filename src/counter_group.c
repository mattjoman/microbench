#include <stdlib.h>

#include "../include/counter_group.h"

static const char ctr_grp_ipc_description[] =
"This counter group is used to\n"
"calculate instructions per cycle (IPC).\n"
"It includes the following counters:\n";

static counter_grp_t ctr_grp_ipc = {
    .id = COUNTER_GRP_IPC,
    .name = "COUNTER_GRP_IPC",
    .description = ctr_grp_ipc_description,
    .size = MAX_COUNTER_GRP_SIZE,
    .counter_ids = {
        COUNTER_CPU_CYCLES,
        COUNTER_REF_CPU_CYCLES,
        COUNTER_INSTRUCTIONS,
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
