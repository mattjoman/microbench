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

static void print_table_row(metric_t metric)
{
    char name_buf[MAX_NAME_LEN];
    char min_buf[TABLE_COLUMN_WIDTH];
    char max_buf[TABLE_COLUMN_WIDTH];
    char median_buf[TABLE_COLUMN_WIDTH];

    if (metric.metric_type == METRIC_TYPE_COUNTER) {

        counter_t ctr = *get_counter(metric.metric.counter.counter_id);

        sprintf(name_buf, "%s", ctr.name);
        sprintf(min_buf, "%ld", metric.metric.counter.min);
        sprintf(max_buf, "%ld", metric.metric.counter.max);
        sprintf(median_buf, "%ld", metric.metric.counter.median);

    } else {

        sprintf(name_buf, "%s", "RATIO");
        sprintf(min_buf, "%f", metric.metric.ratio.min);
        sprintf(max_buf, "%f", metric.metric.ratio.max);
        sprintf(median_buf, "%f", metric.metric.ratio.median);

    }

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

void run_report(analysis_t analysis)
{
    metric_t metric;

    printf("\n");

    for (int metric_idx = 0; metric_idx < analysis.n_metrics; metric_idx++) {

        metric = analysis.metrics[metric_idx];

        print_table_row(metric);
    }

    printf("\n");
}
