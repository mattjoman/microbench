#ifndef METRIC_GRP_H
#define METRIC_GRP_H

#include <stdint.h>
#include <stddef.h>

#include "./cyclops.h"
#include "./registry.h"
#include "./experiment.h"

/*** METRIC GROUPS ***/

typedef struct metric_grp {
    const char *name;
    metric_backend_id_t backend;
    int n_metrics;
    metric_id_t *metrics;
} metric_grp_t;

#define REGISTER_MG(mg_ptr) \
    static void __attribute((constructor)) _registry_register_object(void) { \
        registry_register_object((void *)mg_ptr, REG_ID_METRIC_GRP); \
    }

const metric_t *metric_get_by_id(metric_id_t id);
const metric_t *mg_get_nth_metric_by_type(metric_grp_t *mg,
                                          int n,
                                          metric_type_t type);

metric_grp_t *mg_get_by_name(const char *name);

int mg_n_metrics_by_type(metric_grp_t *mg, metric_type_t type);

// TODO: move print_metric_grp_guide() to core/cli.c
void print_metric_grp_guide(void);

/** METRIC BACKEND ***/

typedef struct batch batch_t;

typedef void (*bench_func_t)(
    batch_t *,
    void (*)(void)
);

typedef struct {
    metric_backend_id_t id;
    bench_func_t bench_func;
} metric_backend_t;

metric_backend_t *metric_backend_get(metric_backend_id_t id);

#endif
