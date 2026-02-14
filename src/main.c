#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "../include/experiment.h"
#include "../include/workload.h"

int main(int argc, char *argv[]) {

    int workload_id = -1;
    int egroup_id = -1;

    static struct option long_opts[] = {
        {"workload",    required_argument, 0, 'w'},
        {"egroup", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    int opt;

    while ((opt = getopt_long(argc, argv, "w:e:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'w':
                workload_id = atoi(optarg);
                break;
            case 'e':
                egroup_id = atoi(optarg);
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

    if (egroup_id < 0 || egroup_id >= NUMBER_OF_EVENT_GROUPS) {
        fprintf(stderr, "Invalid Event Group: %d\n", egroup_id);
        return 1;
    }

    run_experiment(workload_id, egroup_id);

    return 0;
}
