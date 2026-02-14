#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "../include/batch.h"
#include "../include/workload.h"

int main(int argc, char *argv[]) {

    int workload_id = -1;
    int ctr_grp_id = -1;

    static struct option long_opts[] = {
        {"workload",    required_argument, 0, 'w'},
        {"ctr-grp", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    int opt;

    while ((opt = getopt_long(argc, argv, "w:e:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'w':
                workload_id = atoi(optarg);
                break;
            case 'e':
                ctr_grp_id = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage\n");
                return 1;
        }
    }

    if (workload_id < 0 || workload_id >= NUMBER_OF_WORKLOADS) {
        fprintf(stderr, "Invalid Workload: %d\n", workload_id);
        return 1;
    }

    if (ctr_grp_id < 0 || ctr_grp_id >= NUMBER_OF_COUNTER_GRPS) {
        fprintf(stderr, "Invalid Counter Group: %d\n", ctr_grp_id);
        return 1;
    }

    run_batch(workload_id, ctr_grp_id);

    return 0;
}
