#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "../include/batch.h"
#include "../include/workload.h"
#include "../include/metric.h"

static const char help_text[] =
"Usage: microbench [OPTIONS]\n"
"\n"
"Options:\n"
"  -h, --help                       Display this message\n"
"  -w, --workload WORKLOAD          Select workload to benchmark\n"
"  -m, --metric-group GROUP         Select a group of metrics to record\n"
"\n"
"Workloads:\n"
"  contiguous-array                 Process a contiguous in array\n"
"  scattered-array                  Process an array of heap in pointers\n"
"\n"
"Metric groups:\n"
"  ipc                              Record cycles & instructions to get ipc\n"
"";

int main(int argc, char *argv[]) {

    char *workload_str = NULL;
    char *metric_grp_str  = NULL;

    static struct option long_opts[] = {
        {"help",    required_argument, 0, 'h'},
        {"workload",    required_argument, 0, 'w'},
        {"metric-grp", required_argument, 0, 'm'},
        {0, 0, 0, 0}
    };

    int opt;

    while ((opt = getopt_long(argc, argv, "w:m:h", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                fputs(help_text, stdout);
                return 0;
            case 'w':
                workload_str = optarg;
                break;
            case 'm':
                metric_grp_str = optarg;
                break;
            default:
                fprintf(stderr, "Usage 1\n");
                return 1;
        }
    }

    int workload_id = -1;
    int metric_grp_id = -1;

    if (strcmp(workload_str, "contiguous-array") == 0) {
        workload_id = WL_CONTIGUOUS_ARRAY;
    } else if (strcmp(workload_str, "scattered-array") == 0) {
        workload_id = WL_SCATTERED_ARRAY;
    }

    if (strcmp(metric_grp_str, "ipc") == 0) {
        metric_grp_id = METRIC_GRP_IPC;
    }

    if (workload_id < 0 || metric_grp_id < 0) {
        fprintf(stderr, "Usage 2\n");
        return 1;
    }

    batch_conf_t batch_conf = {
        .warmup_runs = 5,
        .batch_runs = MAX_BATCH_SIZE,
        .workload_id = workload_id,
        .metric_grp_id = metric_grp_id,
    };
    run_batch(batch_conf);

    return 0;
}
