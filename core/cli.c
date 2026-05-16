#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "../include/cli.h"
#include "../include/cyclops.h"

// TODO: MAX_WL_ARGS should probably be in include/cyclops.h
#define MAX_WL_ARGS 5

enum {
    OPT_BATCH_CSV = 256,
    OPT_PARAM_SWEEP_CSV = 257,
};

static const char help_text[] =
"Usage: ./cyclops [OPTIONS]\n"
"\n"
"Options:\n\n"
"  -h, --help                       Display this message\n"
"\n"
"  -w, --workload WORKLOAD          Select workload to benchmark\n"
"\n"
"  -m, --metric-group GROUP         Select a group of metrics to record\n"
"\n"
"  -r, --batch-runs RUNS            Number of runs in the batch\n"
"\n"
"  -u, --warmup-runs RUNS           Number of warmup runs\n"
"\n"
"  -p, --param KEY=VAL              Set a custom workload parameter for the\n"
"                                   batch(es)\n"
"\n"
"  -s, --param-sweep KEY=LOW:HIGH:STEP\n"
"                                   Sweep the selected workload parameter\n"
"                                   through a range of values (one batch\n"
"                                   will be run for each param value)\n"
"\n"
"  --batch-csv                      Write run-level data to csv files for\n"
"                                   each batch\n"
"\n"
"  --param-sweep-csv                Write aggregated batch data to\n"
"                                   'param_sweep.csv'\n"
"\n";

static struct option long_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"workload", required_argument, 0, 'w'},
    {"metric-grp", required_argument, 0, 'm'},
    {"batch-runs", required_argument, 0, 'r'},
    {"warmup-runs", required_argument, 0, 'u'},
    {"param", required_argument, 0, 'p'},
    {"param-sweep", required_argument, 0, 's'},

    /* long opts only */
    {"batch-csv", no_argument, 0, OPT_BATCH_CSV},
    {"param-sweep-csv", no_argument, 0, OPT_PARAM_SWEEP_CSV},

    {0, 0, 0, 0}
};

cyclops_cfg_t *cli_cfg_init(int argc, char *argv[])
{
    bool display_help = false;
    static char *workload_str = NULL;
    static char *metric_grp_str  = NULL;
    unsigned long long batch_runs = 1;
    unsigned long long warmup_runs = 0;
    int n_wl_params = 0;
    static char *wl_param_keys[MAX_WL_ARGS];
    static char *wl_param_args[MAX_WL_ARGS];

    static char *wl_param_sweep_key = NULL;
    static char *wl_param_sweep_low = NULL;
    static char *wl_param_sweep_high = NULL;
    static char *wl_param_sweep_step = NULL;

    bool batch_csv = false;
    bool param_sweep_csv = false;

    int opt;
    char *key;
    char *eq;
    char *colon_1;
    char *colon_2;
    while ((opt = getopt_long(argc, argv, "hw:m:r:u:p:s:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                display_help = true;
                break;
            case 'w':
                workload_str = optarg;
                break;
            case 'm':
                metric_grp_str = optarg;
                break;
            case 'r':
                batch_runs = strtoull(optarg, NULL, 10);
                break;
            case 'u':
                warmup_runs = strtoull(optarg, NULL, 10);
                break;
            case 'p':
                if (n_wl_params >= MAX_WL_ARGS) {
                    fprintf(stderr, "Too many workload param args\n");
                    return NULL;
                }

                key = optarg;
                eq = strchr(key, '=');

                if (!eq) {
                    fprintf(stderr, "Invalid param format: %s\n", key);
                    return NULL;
                }
                *eq = '\0'; // split into two strings
                wl_param_keys[n_wl_params] = key;
                wl_param_args[n_wl_params] = eq + 1;
                eq = NULL;
                n_wl_params++;
                break;
            case 's':
                key = optarg;

                eq = strchr(key, '=');
                if (!eq) {
                    fprintf(stderr, "Invalid param format: %s\n", key);
                    return NULL;
                }
                *eq = '\0'; // split into two strings
                wl_param_sweep_key = key;
                wl_param_sweep_low = eq + 1;

                colon_1 = strchr(wl_param_sweep_low, ':');
                if (!colon_1) {
                    fprintf(stderr, "Invalid param format: %s\n", key);
                    return NULL;
                }
                *colon_1 = '\0';
                wl_param_sweep_high = colon_1 + 1;
                colon_1 = NULL;

                colon_2 = strchr(wl_param_sweep_high, ':');
                if (!colon_2) {
                    fprintf(stderr, "Invalid param format: %s\n", key);
                    return NULL;
                }
                *colon_2 = '\0';
                wl_param_sweep_step = colon_2 + 1;
                colon_2 = NULL;

                break;
            case OPT_BATCH_CSV:
                batch_csv = true;
                break;
            case OPT_PARAM_SWEEP_CSV:
                param_sweep_csv = true;
                break;
            default:
                fprintf(stderr, "Usage 1\n");
                return NULL;
        }
    }

    cyclops_cfg_t *cfg = calloc(1, sizeof(cyclops_cfg_t));
    if (!cfg) {
        perror("Failed to allocate memory for the cyclops config struct");
        exit(1);
    }

    cfg->display_help = display_help;
    cfg->warmup_runs = warmup_runs;
    cfg->batch_runs = batch_runs;
    cfg->wl_name = workload_str;
    cfg->mg_name = metric_grp_str;
    cfg->n_wl_params = n_wl_params;
    cfg->wl_param_keys = wl_param_keys;
    cfg->wl_param_args = wl_param_args;
    cfg->ps_wl_param_key = wl_param_sweep_key;
    cfg->ps_wl_param_low = wl_param_sweep_low;
    cfg->ps_wl_param_high = wl_param_sweep_high;
    cfg->ps_wl_param_step = wl_param_sweep_step;
    cfg->batch_csv = batch_csv;
    cfg->param_sweep_csv = param_sweep_csv;

    return cfg;
}

void cli_cfg_destroy(cyclops_cfg_t *cli_cfg)
{
    free(cli_cfg);
}

bool cli_cfg_validate(cyclops_cfg_t *cli_cfg)
{
    /* Help requested: other args are irrelevant */
    if (cli_cfg->display_help) {
        return true;
    }

    if (!cli_cfg->wl_name) {
        fprintf(stderr, "Usage: Please select a workload\n");
        return false;
    }

    if (!cli_cfg->mg_name) {
        fprintf(stderr, "Usage: Please select a metric group\n");
        return false;
    }

    return true;
}

void print_main_help_text()
{
    fputs(help_text, stdout);
}
