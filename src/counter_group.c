#include <stdlib.h>

#include "../include/counter.h"
#include "../include/counter_group.h"

const char *counter_grp_names[NUMBER_OF_COUNTER_GRPS] = {
    [COUNTER_GRP_IPC]               = "COUNTER_GRP_IPC",
};

static const ctr_grp_t ctr_grp_ipc = {
    .id = COUNTER_GRP_IPC,
    .size = MAX_COUNTER_GRP_SIZE,
    .ctr_ids[0] = COUNTER_CPU_CYCLES,
    .ctr_ids[1] = COUNTER_REF_CPU_CYCLES,
    .ctr_ids[2] = COUNTER_INSTRUCTIONS,
};

const ctr_grp_t *get_ctr_grp(int ctr_grp_id)
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
