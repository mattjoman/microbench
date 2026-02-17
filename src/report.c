#include <stdio.h>
#include <string.h>

#include "../include/report.h"
#include "../include/counter_group.h"
#include "../include/metric.h"

#define TABLE_COLUMN_WIDTH 24

static void print_table_cell_right_align(char *text)
{
    int text_len = strlen(text);
    char cell_buf[TABLE_COLUMN_WIDTH];

    memset(cell_buf, ' ', sizeof(cell_buf));
    memcpy(&cell_buf[TABLE_COLUMN_WIDTH - text_len], text, text_len);

    printf("%s", cell_buf);
}

static void print_counter_table_row(counter_metric_t metric)
{
    char name_buf[MAX_NAME_LEN];
    char min_buf[TABLE_COLUMN_WIDTH];
    char max_buf[TABLE_COLUMN_WIDTH];
    char median_buf[TABLE_COLUMN_WIDTH];

    sprintf(name_buf, "%s", metric.name);
    sprintf(min_buf, "%ld", metric.min);
    sprintf(max_buf, "%ld", metric.max);
    sprintf(median_buf, "%ld", metric.median);

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

static void print_ratio_table_row(ratio_metric_t metric)
{
    char name_buf[MAX_NAME_LEN];
    char min_buf[TABLE_COLUMN_WIDTH];
    char max_buf[TABLE_COLUMN_WIDTH];
    char median_buf[TABLE_COLUMN_WIDTH];

    sprintf(name_buf, "%s", metric.name);
    sprintf(min_buf, "%f", metric.min);
    sprintf(max_buf, "%f", metric.max);
    sprintf(median_buf, "%f", metric.median);

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

void run_report(batch_res_t batch_res)
{
    counter_metric_t ctr_metric;

    printf("\n");

    for (int i = 0; i < batch_res.n_ctr_metrics; i++) {

        ctr_metric = batch_res.ctr_metrics[i];

        print_counter_table_row(ctr_metric);
    }
    
    print_ratio_table_row(batch_res.ratio_metrics[0]);

    printf("\n");
}
