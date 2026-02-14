#include <stdio.h>

#include "../include/report.h"
#include "../include/event.h"
#include "../include/analysis.h"

void run_report(event_group_t egroup, analysis_t analysis)
{
    const char *egroup_name;
    int event_id;

    egroup_name = event_group_names[egroup.id];

    printf("\nEvent Group: %s\n\n", egroup_name);

    for (int e = 0; e < MAX_EVENT_GROUP_SIZE; e++) {
        event_id = analysis.event_aggs[e].event_id;
        printf("%s:\n", metric_names[event_id]);
        printf("Min:    %ld\n", analysis.event_aggs[e].min);
        printf("Median: %ld\n", analysis.event_aggs[e].median);
        printf("Max:    %ld\n\n", analysis.event_aggs[e].max);
    }

    printf("RATIO:\n");
    printf("Min:    %f\n", analysis.ratio_agg.min);
    printf("Median: %f\n", analysis.ratio_agg.median);
    printf("Max:    %f\n\n", analysis.ratio_agg.max);
}
