#include <stdlib.h>
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

static void print_raw_metric_table_row(raw_metric_t metric)
{
    char name_buf[32];
    char min_buf[32];
    char max_buf[32];
    char median_buf[32];

    snprintf(name_buf, sizeof(name_buf), "%s", raw_metric_confs[metric.id]);
    snprintf(min_buf, sizeof(min_buf), "%ld", metric.agg.min);
    snprintf(max_buf, sizeof(max_buf), "%ld", metric.agg.max);
    snprintf(median_buf, sizeof(median_buf), "%ld", metric.agg.median);

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

    snprintf(name_buf, sizeof(name_buf), "%s", ratio_confs[metric.id].name);
    snprintf(min_buf, sizeof(min_buf), "%.6f", metric.agg.min);
    snprintf(max_buf, sizeof(max_buf), "%.6f", metric.agg.max);
    snprintf(median_buf, sizeof(median_buf), "%.6f", metric.agg.median);

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

static void print_batch_info(batch_conf_t batch_conf)
{
    printf("    Warmup runs: %llu\n", batch_conf.warmup_runs);
    printf("    Batch runs:  %llu\n", batch_conf.batch_runs);
    putchar('\n');
}

void run_report(batch_conf_t batch_conf, batch_data_t *batch_data)
{
    raw_metric_t raw_metric;
    ratio_metric_t ratio_metric;

    printf("\n");
    print_batch_info(batch_conf);
    print_table_column_headers();

    for (int i = 0; i < batch_data->n_raw_metrics; i++) {
        raw_metric = batch_data->raw_metrics[i];
        print_raw_metric_table_row(raw_metric);
    }

    for (int i = 0; i < batch_data->n_ratios; i++) {
        ratio_metric = batch_data->ratios[i];
        print_ratio_table_row(ratio_metric);
    }

    printf("\n");
}

static void write_batch_metadata(FILE *file, batch_conf_t batch_conf)
{
    fprintf(file, "#workload=%s\n", batch_conf.wl->name);
    fprintf(file, "#metric-group=%s\n",
                               metric_grps[batch_conf.metric_grp_id].name);
    fprintf(file, "#warmup-runs=%llu\n", batch_conf.warmup_runs);
    fprintf(file, "#batch-runs=%llu\n", batch_conf.batch_runs);

    workload_t *wl = batch_conf.wl;
    if (!wl->params) {
        return;
    }

    for (int i = 0; i < wl->n_params; i++) {
        fprintf(file, "#workload-params.%s=", wl->params[i].key);
        fprintf(file, "%llu\n", wl_get_param(wl, wl->params[i].key));
    }
}

void timer_batch_to_csv(batch_conf_t batch_conf, batch_data_t *batch_data)
{
    FILE *file = fopen("batch.csv", "w");
    if (!file) {
        perror("Failed to open csv file");
        exit(1);
    }

    write_batch_metadata(file, batch_conf);

    fprintf(file, "%s\n", raw_metric_confs[batch_data->raw_metrics[0].id]);

    for (unsigned long long r = 0; r < batch_conf.batch_runs; r++) {
        fprintf(file, "%ld\n", batch_data->raw_metrics[0].run_vals[r]);
    }

    fclose(file);
}

void perf_batch_to_csv(batch_conf_t batch_conf, batch_data_t *batch_data)
{
    FILE *file = fopen("batch.csv", "w");
    if (!file) {
        perror("Failed to open csv file");
        exit(1);
    }

    write_batch_metadata(file, batch_conf);

    fputs("TIME_ENABLED,", file);
    fputs("TIME_RUNNING,", file);

    for (int m = 0; m < batch_data->n_raw_metrics; m++) {
        fprintf(file, "%s,", raw_metric_confs[batch_data->raw_metrics[m].id]);
    }

    for (int m = 0; m < batch_data->n_ratios; m++) {
        fprintf(file, "%s,", ratio_confs[batch_data->ratios[m].id].name);
    }

    fputc('\n', file);

    for (unsigned long long i = 0; i < batch_conf.batch_runs; i++) {
        fprintf(file, "%ld,", batch_data->time_enabled.run_vals[i]);
        fprintf(file, "%ld,", batch_data->time_running.run_vals[i]);

        for (int m = 0; m < batch_data->n_raw_metrics; m++) {
            fprintf(file, "%ld,", batch_data->raw_metrics[m].run_vals[i]);
        }

        for (int m = 0; m < batch_data->n_ratios; m++) {
            fprintf(file, "%.6f,", batch_data->ratios[m].run_vals[i]);
        }

        fputc('\n', file);
    }

    fclose(file);
}
