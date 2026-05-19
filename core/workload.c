#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/workload.h"

/*
void register_workload(workload_t *wl)
{
    workloads = realloc(workloads, (n_workloads + 1) * sizeof(*workloads));
    workloads[n_workloads++] = wl;
}
*/

static workload_registry_t wl_registry = {
    .n_registered = 0,
    .registry = NULL,
};

void wl_register(workload_t *wl)
{
    wl_registry.registry = realloc(wl_registry.registry,
                                   (wl_registry.n_registered + 1)
                                   * sizeof(workload_t *));
    wl_registry.registry[wl_registry.n_registered++] = wl;
}

workload_registry_t *wl_registry_get_registry(void)
{
    return &wl_registry;
}

void print_workload_guide(void)
{
    printf("Workloads:\n\n");

    for (size_t i = 0; i < wl_registry.n_registered; i++) {
        workload_t *wl = wl_registry.registry[i];
        printf("  %s\n", wl->name);
    }
    printf("\n");
}

workload_t *wl_get_by_name(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < wl_registry.n_registered; i++) {
        workload_t *wl = wl_registry.registry[i];

        if (strcmp(name, wl->name) == 0) {
            return wl;
        }
    }
    return NULL;
}

unsigned long long wl_param_get_val(wl_param_t *wl_param)
{
    if (wl_param->arg) {
        return strtoull(wl_param->arg, NULL, 10);
    }
    return strtoull(wl_param->default_value, NULL, 10);
}

/*
 * Returns the workload parameter value given a parameter key.
 *
 * If no arg is provided for this param by the user (with --param key=value),
 * use the param's default value.
 */
unsigned long long wl_get_param_val(workload_t *wl, const char *key)
{
    for (int i = 0; i < wl->n_params; i++) {
        wl_param_t *wl_param = &wl->params[i];

        if (strcmp(wl_param->key, key) == 0) {
            return wl_param_get_val(wl_param);
        };
    }

    fprintf(stderr, "Invalid workload parameter '%s'\n", key);
    exit(1);
    return 0;
}

void wl_set_param_val(workload_t *wl, const char *key, const char *arg)
{
    if (!wl->params) {
        fprintf(stderr, "Workload has no parameters\n");
        return;
    }

    for (int i = 0; i < wl->n_params; i++) {
        if (strcmp(wl->params[i].key, key) == 0) {
            wl->params[i].arg = arg;
            return;
        }
    }

    fprintf(stderr, "Invalid workload parameter '%s'\n", key);
}
