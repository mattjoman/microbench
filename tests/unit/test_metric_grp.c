#include <assert.h>
#include <stddef.h>

#include "../include/metric_grp.h"

int main()
{
    registry_t *reg = registry_get_reg_by_id(REG_ID_METRIC_GRP);
    metric_grp_t *mg;

    assert(reg != NULL);

    for (size_t i = 0; i < reg->n_registered; i++) {
        mg = (metric_grp_t *)reg->registry[i];

        for (int j = 0; j < mg->n_metrics; j++) {
            const metric_t *m = metric_get_by_id(mg->metrics[j]);
            assert(mg->backend == m->backend);
        }
    }

    return 0;
}
