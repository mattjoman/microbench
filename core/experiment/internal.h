#ifndef EXPERIMENT_INTERNAL_H
#define EXPERIMENT_INTERNAL_H

#include "../../include/experiment.h"

///////////////////////////////////////////////////////////////////////////////
/// DATA PROCESSING

#include <stdint.h>

double_agg_t aggregate_double(double array[], unsigned long long size);

void calc_ratios(double results[], const double numerators[],
                                   const double denominators[],
                                   unsigned long long size);

///////////////////////////////////////////////////////////////////////////////
/// BATCH

#include <stdbool.h>
#include "../../include/metric_grp.h"

batch_t *batch_init(cyclops_cfg_t *cyclops_cfg);
void destroy_batch_data(batch_t *data);
void batch_param_sweep_run(batch_t *batch_data,
                           unsigned long long batch_no);
metric_data_t *batch_get_metric_data(batch_t *data,
                                     metric_id_t metric_id);

///////////////////////////////////////////////////////////////////////////////
/// REPORT

#include <stdbool.h>

#include "../../include/experiment.h"

void run_report(batch_t *batch_data);
void batch_to_csv(batch_t *batch_data, unsigned long long batch_no);
void param_sweep_to_csv(param_sweep_t *ps);

#endif
