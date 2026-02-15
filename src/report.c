#include <stdio.h>

#include "../include/report.h"
#include "../include/counter_group.h"
#include "../include/analysis.h"

void run_report(ctr_grp_t ctr_grp, analysis_t analysis)
{
    int ctr_id;

    printf("\nCounter Group: %s\n\n", ctr_grp.name);

    for (int c = 0; c < ctr_grp.size; c++) {
        ctr_id = analysis.ctr_aggs[c].ctr_id;
        printf("%s:\n", ctr_grp.counters[ctr_id].name);
        printf("Min:    %ld\n", analysis.ctr_aggs[c].min);
        printf("Median: %ld\n", analysis.ctr_aggs[c].median);
        printf("Max:    %ld\n\n", analysis.ctr_aggs[c].max);
    }

    printf("RATIO:\n");
    printf("Min:    %f\n", analysis.ratio_agg.min);
    printf("Median: %f\n", analysis.ratio_agg.median);
    printf("Max:    %f\n\n", analysis.ratio_agg.max);
}
