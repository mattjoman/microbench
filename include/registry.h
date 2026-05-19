#ifndef REGISTRY_H
#define REGISTRY_H

typedef enum {
    REG_ID_WORKLOAD,
    REG_ID_METRIC_GRP,
    REG_ID_METRIC_BACKEND,
    N_REGISTRIES,
} registry_id_t;

typedef struct {
    registry_id_t id;
    size_t n_registered;
    void **registry;
} registry_t;

/*
 * Returns a pointer to the registry with the given id
 */
registry_t *registry_get_reg_by_id(registry_id_t id);

void registry_register_object(void *object, registry_id_t reg_id);

#endif
