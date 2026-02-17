#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "../include/batch.h"
#include "../include/counter_group.h"
#include "../include/workload.h"

int main(int argc, char *argv[]) {

    char *workload_str = NULL;
    char *ctr_grp_str  = NULL;

    static struct option long_opts[] = {
        {"workload",    required_argument, 0, 'w'},
        {"ctr-grp", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    int opt;

    while ((opt = getopt_long(argc, argv, "w:c:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'w':
                //workload_id = atoi(optarg);
                workload_str = optarg;
                break;
            case 'c':
                ctr_grp_str = optarg;
                //ctr_grp_id = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage 1\n");
                return 1;
        }
    }

    int workload_id = -1;
    int ctr_grp_id = -1;

    if (strcmp(workload_str, "contiguous-array") == 0) {
        workload_id = WL_CONTIGUOUS_ARRAY;
    }

    if (strcmp(ctr_grp_str, "ipc") == 0) {
        ctr_grp_id = COUNTER_GRP_IPC;
    }

    if (workload_id < 0 || ctr_grp_id < 0) {
        fprintf(stderr, "Usage 2\n");
        return 1;
    }

    run_batch(workload_id, ctr_grp_id);

    return 0;
}
