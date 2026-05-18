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
void batch_destroy(batch_t *b);
void batch_param_sweep_run(batch_t *b, unsigned long long batch_no);
batch_metric_t *batch_get_batch_metric_by_id(batch_t *b,
                                             metric_id_t metric_id);

///////////////////////////////////////////////////////////////////////////////
/// REPORT

#include <stdbool.h>

#include "../../include/experiment.h"

void run_report(batch_t *b);
void batch_to_csv(batch_t *b, unsigned long long batch_no);
void ps_to_csv(param_sweep_t *ps);

#endif
