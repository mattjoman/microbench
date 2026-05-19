#ifndef WORKLOAD_H
#define WORKLOAD_H

#include "./registry.h"

typedef struct {
    const char *key;
    const char *arg;
    const char *default_value;
} wl_param_t;

typedef struct workload workload_t;
typedef struct workload {
    const char* name;

    const int n_params;
    wl_param_t *params;

    void (*init)(workload_t *wl);
    void (*clean)(void);
    void (*workload)(void);
} workload_t;

typedef struct {
    size_t n_registered;
    workload_t **registry;
} workload_registry_t;

#define REGISTER_WORKLOAD(wl_ptr) \
    static void __attribute((constructor)) _registry_register_object(void) { \
        registry_register_object((void *)wl_ptr, REG_ID_WORKLOAD); \
    }

void print_workload_guide(void);

workload_t *wl_get_by_name(const char *name);

unsigned long long wl_param_get_val(wl_param_t *wl_param);
unsigned long long wl_get_param_val(workload_t *wl, const char *key);
void wl_set_param_val(workload_t *wl, const char *key, const char *arg);

#endif
