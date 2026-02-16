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

void run_report(analysis_t analysis)
{
    aggregate_t agg;

    printf("\n");

    for (int agg_idx = 0; agg_idx < analysis.n_aggs; agg_idx++) {

        agg = analysis.aggregates[agg_idx];

        if (agg.agg_type == AGG_TYPE_RATIO)
            continue;

        counter_agg_t ctr_agg = analysis.aggregates[agg_idx].agg_data.counter_agg;
        counter_t ctr = *get_counter(ctr_agg.counter_id);

        print_table_row(ctr_agg, ctr.name);
    }

    printf("\n");
}
