#include <stdio.h>

#include "../include/report.h"
#include "../include/counter.h"
#include "../include/analysis.h"

void run_report(ctr_grp_t ctr_grp, analysis_t analysis)
{
    const char *ctr_grp_name;
    int ctr_id;

    ctr_grp_name = counter_grp_names[ctr_grp.id];

    printf("\nCounter Group: %s\n\n", ctr_grp_name);

    for (int e = 0; e < MAX_COUNTER_GRP_SIZE; e++) {
        ctr_id = analysis.ctr_aggs[e].ctr_id;
        printf("%s:\n", counter_names[ctr_id]);
        printf("Min:    %ld\n", analysis.ctr_aggs[e].min);
        printf("Median: %ld\n", analysis.ctr_aggs[e].median);
        printf("Max:    %ld\n\n", analysis.ctr_aggs[e].max);
    }

    printf("RATIO:\n");
    printf("Min:    %f\n", analysis.ratio_agg.min);
    printf("Median: %f\n", analysis.ratio_agg.median);
    printf("Max:    %f\n\n", analysis.ratio_agg.max);
}
