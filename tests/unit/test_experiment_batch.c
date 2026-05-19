#include <assert.h>
#include <stddef.h>

#include "../include/experiment.h"
#include "../core/experiment/internal.h"

int main()
{
    /* init batch */

    cyclops_cfg_t cli_cfg = {
        .wl_name = "BRANCH",
        .mg_name = "IPC",
        .batch_runs = 1,
    };

    batch_t *b = batch_init(&cli_cfg);
    assert(b != NULL);

    unsigned long long run = 0;

    /* set run val for METRIC_INSTRUCTIONS */

    batch_metric_t *bm_instructions;
    bm_instructions = batch_get_batch_metric_by_id(b, METRIC_INSTRUCTIONS);

    assert(bm_instructions != NULL);
    assert(bm_instructions->metric->id == METRIC_INSTRUCTIONS);

    batch_metric_set_run_val(bm_instructions, run, 90.0);

    assert(bm_instructions->run_vals[run] == 90.0);

    /* set run val for METRIC_CPU_CYCLES */

    batch_metric_t *bm_cpu_cycles;
    bm_cpu_cycles = batch_get_batch_metric_by_id(b, METRIC_CPU_CYCLES);

    assert(bm_cpu_cycles != NULL);
    assert(bm_cpu_cycles->metric->id == METRIC_CPU_CYCLES);

    batch_metric_set_run_val(bm_cpu_cycles, run, 100.0);

    assert(bm_cpu_cycles->run_vals[run] == 100.0);

    /* set run val for METRIC_CPU_CYCLES */

    batch_metric_t *bm_ipc;
    bm_ipc = batch_get_batch_metric_by_id(b, METRIC_INSTRUCTIONS_PER_CYCLE);

    assert(bm_ipc != NULL);
    assert(bm_ipc->metric->id == METRIC_INSTRUCTIONS_PER_CYCLE);

    calc_ratios(bm_ipc->run_vals,
                bm_instructions->run_vals,
                bm_cpu_cycles->run_vals,
                b->batch_runs);

    assert(bm_ipc->run_vals[run] == 0.9);

    /* do some adversarial stuff */

    batch_metric_t *bm;

    // metric not in the 'IPC' metric group, lookup should return NULL
    bm = batch_get_batch_metric_by_id(b, METRIC_L1D_READ_MISSES);
    assert(bm == NULL);

    /* cleanup */

    batch_destroy(b);

    return 0;
}
