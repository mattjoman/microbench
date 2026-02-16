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

static void print_table_row(aggregate_t agg)
{
    char name_buf[MAX_NAME_LEN];
    char min_buf[TABLE_COLUMN_WIDTH];
    char max_buf[TABLE_COLUMN_WIDTH];
    char median_buf[TABLE_COLUMN_WIDTH];

    if (agg.agg_type == AGG_TYPE_COUNTER) {

        counter_t ctr = *get_counter(agg.agg_data.counter_agg.counter_id);

        sprintf(name_buf, "%s", ctr.name);
        sprintf(min_buf, "%ld", agg.agg_data.counter_agg.min);
        sprintf(max_buf, "%ld", agg.agg_data.counter_agg.max);
        sprintf(median_buf, "%ld", agg.agg_data.counter_agg.median);

    } else {

        sprintf(name_buf, "%s", "RATIO");
        sprintf(min_buf, "%f", agg.agg_data.ratio_agg.min);
        sprintf(max_buf, "%f", agg.agg_data.ratio_agg.max);
        sprintf(median_buf, "%f", agg.agg_data.ratio_agg.median);

    }

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

        print_table_row(agg);
    }

    printf("\n");
}
