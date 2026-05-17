#include <assert.h>
#include <stddef.h>

#include "../include/metric_grp.h"

int main()
{
    metric_grp_registry *mg_registry = mg_registry_get_registry();
    assert(mg_registry != NULL);

    for (size_t i = 0; i < mg_registry->n_registered; i++) {
        metric_grp_t *mg = mg_registry->registry[i];
        for (int j = 0; j < mg->n_metrics; j++) {
            const metric_t *m = metric_get_by_id(mg->metrics[j]);
            assert(mg->backend == m->backend);
        }
    }

    return 0;
}
