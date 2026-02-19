#include <stdio.h>
#include <string.h>

#include "../include/report.h"
#include "../include/metric.h"

#define TABLE_COLUMN_WIDTH 18

static void print_table_cell_right_align(const char *text)
{
    int text_len = strlen(text);

    if (text_len > TABLE_COLUMN_WIDTH)
        text_len = TABLE_COLUMN_WIDTH;

    char cell_buf[TABLE_COLUMN_WIDTH + 1] = { '\0' };

    memset(cell_buf, ' ', sizeof(cell_buf) - 1);
    memcpy(&cell_buf[TABLE_COLUMN_WIDTH - text_len], text, text_len);

    printf("%s", cell_buf);
}

static void print_counter_table_row(counter_metric_t metric)
{
    char name_buf[32];
    char min_buf[32];
    char max_buf[32];
    char median_buf[32];

    //snprintf(name_buf, sizeof(name_buf), "%s", metric.name);
    snprintf(name_buf, sizeof(name_buf), "%s", metric_names[metric.id]);
    snprintf(min_buf, sizeof(min_buf), "%ld", metric.min);
    snprintf(max_buf, sizeof(max_buf), "%ld", metric.max);
    snprintf(median_buf, sizeof(median_buf), "%ld", metric.median);

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

static void print_ratio_table_row(ratio_metric_t metric)
{
    char name_buf[32];
    char min_buf[32];
    char max_buf[32];
    char median_buf[32];

    //snprintf(name_buf, sizeof(name_buf), "%s", metric.name);
    snprintf(name_buf, sizeof(name_buf), "%s", metric_names[metric.id]);
    snprintf(min_buf, sizeof(min_buf), "%.2f", metric.min);
    snprintf(max_buf, sizeof(max_buf), "%.2f", metric.max);
    snprintf(median_buf, sizeof(median_buf), "%.2f", metric.median);

    print_table_cell_right_align(name_buf);
    print_table_cell_right_align(min_buf);
    print_table_cell_right_align(max_buf);
    print_table_cell_right_align(median_buf);

    printf("\n");
}

static void print_table_column_headers()
{
    print_table_cell_right_align("Metric");
    print_table_cell_right_align("Min");
    print_table_cell_right_align("Max");
    print_table_cell_right_align("Median");
    printf("\n");

    for (int i = 0; i < TABLE_COLUMN_WIDTH * 4; i++) {
        putchar('=');
    }
    putchar('\n');
}

void run_report(batch_data_t batch_data)
{
    counter_metric_t ctr_metric;

    printf("\n");

    print_table_column_headers();

    for (int i = 0; i < batch_data.metric_grp.n_counters; i++) {

        ctr_metric = batch_data.counters[i];

        print_counter_table_row(ctr_metric);
    }
    
    print_ratio_table_row(batch_data.ratios[0]);

    printf("\n");
}
