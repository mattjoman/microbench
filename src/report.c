#include <stdio.h>
#include <string.h>

#include "../include/report.h"
#include "../include/counter_group.h"
#include "../include/analysis.h"

#define TABLE_COLUMN_WIDTH 24

static void print_table_cell_right_align(char *text)
{
    int text_len = strlen(text);

    char cell_buf[TABLE_COLUMN_WIDTH];
    memset(cell_buf, ' ', sizeof(cell_buf));
    memcpy(&cell_buf[TABLE_COLUMN_WIDTH - text_len], text, text_len);

    printf("%s", cell_buf);
}

static void print_table_row(counter_agg_t ctr_agg, char *row_name)
{
    char name_buf[MAX_NAME_LEN];
    sprintf(name_buf, "%s", row_name);

    char min_buf[TABLE_COLUMN_WIDTH];
    sprintf(min_buf, "%ld", ctr_agg.min);

    char max_buf[TABLE_COLUMN_WIDTH];
    sprintf(max_buf, "%ld", ctr_agg.max);

    char median_buf[TABLE_COLUMN_WIDTH];
    sprintf(median_buf, "%ld", ctr_agg.median);

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

void run_report(counter_grp_t ctr_grp, analysis_t analysis)
{
    int ctr_id;

    printf("\nCounter Group: %s\n\n", ctr_grp.name);

    for (int c = 0; c < ctr_grp.size; c++) {
        ctr_id = analysis.counter_aggs[c].counter_id;
        printf("%s:\n", ctr_grp.counters[ctr_id].name);
        printf("Min:    %ld\n", analysis.counter_aggs[c].min);
        printf("Median: %ld\n", analysis.counter_aggs[c].median);
        printf("Max:    %ld\n\n", analysis.counter_aggs[c].max);
    }

    printf("RATIO:\n");
    printf("Min:    %f\n", analysis.ratio_agg.min);
    printf("Median: %f\n", analysis.ratio_agg.median);
    printf("Max:    %f\n\n", analysis.ratio_agg.max);

    for (int c = 0; c < ctr_grp.size; c++) {
        counter_agg_t ctr_agg = analysis.counter_aggs[c];
        counter_t ctr = *get_counter(ctr_agg.counter_id);
        print_table_row(ctr_agg, ctr.name);
    }
    printf("\n");
}
