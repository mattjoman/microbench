#include "../include/cli.h"
#include "../include/experiment.h"
#include "../include/workload.h"
#include "../include/metric_grp.h"

int main(int argc, char *argv[])
{
    cyclops_cfg_t *cli_cfg = cli_cfg_init(argc, argv);
    if (!cli_cfg) {
        goto error;
    }

    if (!cli_cfg_validate(cli_cfg)) {
        goto error;
    }

    if (cli_cfg->display_help) {
        print_main_help_text();
        print_workload_guide();
        print_metric_grp_guide();
        goto success;
    }

    if (cli_cfg->ps_wl_param_key) {
        param_sweep_run(cli_cfg);
    } else {
        batch_single_run(cli_cfg);
    }

success:
    cli_cfg_destroy(cli_cfg);
    return 0;

error:
    cli_cfg_destroy(cli_cfg);
    return 1;
}
