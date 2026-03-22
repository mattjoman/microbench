#ifndef REPORT_H
#define REPORT_H

#include "./batch.h"

void run_perf_report(batch_conf_t batch_conf, batch_data_t *batch_data);
void timer_batch_to_csv(batch_conf_t batch_conf, batch_data_t *batch_data);
void perf_batch_to_csv(batch_conf_t batch_conf, batch_data_t *batch_data);

#endif
