#ifndef REPORT_H
#define REPORT_H

#include <stdbool.h>

#include "./batch.h"

void run_report(batch_data_t *batch_data);
void batch_to_csv(batch_data_t *batch_data, unsigned long long batch_no);
void param_sweep_to_csv(param_sweep_t *ps);

#endif
