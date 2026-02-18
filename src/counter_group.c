#include <stdlib.h>

#include "../include/counter_group.h"

static counter_grp_t ctr_grp_ipc = {
    .id = COUNTER_GRP_IPC,
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
