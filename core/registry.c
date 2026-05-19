#include <stdlib.h>

#include "../include/registry.h"

static registry_t registries[N_REGISTRIES] = {
    [REG_ID_WORKLOAD] = {
        .id = REG_ID_WORKLOAD,
        .n_registered = 0,
        .registry = NULL,
    },
    [REG_ID_METRIC_GRP] = {
        .id = REG_ID_METRIC_GRP,
        .n_registered = 0,
        .registry = NULL,
    },
    [REG_ID_METRIC_BACKEND] = {
        .id = REG_ID_METRIC_BACKEND,
        .n_registered = 0,
        .registry = NULL,
    }
};

registry_t *registry_get_reg_by_id(registry_id_t id)
{
    return &registries[id];
}

void registry_register_object(void *object, registry_id_t id)
{
    registry_t *reg = registry_get_reg_by_id(id);

    reg->registry = realloc(reg->registry,
                            (reg->n_registered + 1) * sizeof(void *));
    reg->registry[reg->n_registered++] = object;
}
