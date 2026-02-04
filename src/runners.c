#include <stdio.h>
#include <string.h>

#include "../include/include.h"

const char *metric_names[NUMBER_OF_METRICS] = {
    [METRIC_CPU_CYCLES]            = "METRIC_CPU_CYCLES",
    [METRIC_INSTRUCTIONS]          = "METRIC_INSTRUCTIONS",
    [METRIC_CACHE_ACCESSES]        = "METRIC_CACHE_ACCESSES",
    [METRIC_CACHE_MISSES]          = "METRIC_CACHE_MISSES",
    [METRIC_L1_CACHE_MISSES]       = "METRIC_L1_CACHE_MISSES",
    [METRIC_BRANCH_INSTRUCTIONS]   = "METRIC_BRANCH_INSTRUCTIONS",
    [METRIC_BRANCH_MISPREDICTIONS] = "METRIC_BRANCH_MISPREDICTIONS",
    [METRIC_PAGE_FAULTS]           = "METRIC_PAGE_FAULTS",
    [METRIC_CPU_CLOCK_NS]          = "METRIC_CPU_CLOCK_NS",
    [METRIC_TASK_CLOCK_NS]         = "METRIC_TASK_CLOCK_NS",
    [METRIC_ALIGNMENT_FAULTS]      = "METRIC_ALIGNMENT_FAULTS",
};

typedef struct metric_agg metric_agg_t;
struct metric_agg {
    uint64_t min;
    uint64_t max;
    uint64_t median;
};

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

static batch_t batch_init(int warmup_runs, int batch_runs, int event_group_size,
                                int event_group[])
{
    if (batch_runs < 1 || batch_runs > MAX_BENCH_BATCH_SIZE)
        abort();

    if (event_group_size < 1 || event_group_size > MAX_EVENT_GROUP_SIZE)
        abort();

    batch_t batch;
    memset(&batch, 0, sizeof(batch_t));

    batch.warmup_runs = warmup_runs;
    batch.batch_runs = batch_runs;
    batch.event_group_size = event_group_size;

    memcpy(batch.event_group, event_group, event_group_size * sizeof(int));

    return batch;
}

static void sort_swap(uint64_t *a, uint64_t *b)
{
    uint64_t tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static int sort_partition(uint64_t array[], int low, int high)
{
    uint64_t pivot;
    int i, j;

    i = low - 1;
    pivot = array[high];

    for (j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            sort_swap(&array[i], &array[j]);
        }
    }

    sort_swap(&array[i + 1], &array[high]);

    return i + 1;
}

static void sort(uint64_t array[], int low, int high)
{
    int p;

    if (low < high) {
        p = sort_partition(array, low, high);
        sort(array, low, p - 1);
        sort(array, p + 1, high);
    }
}

static metric_agg_t metric_agg(uint64_t batch_metric_results[], int batch_runs)
{
    metric_agg_t agg;
    uint64_t array_cpy[MAX_BENCH_BATCH_SIZE];

    memcpy(array_cpy, batch_metric_results, batch_runs * sizeof(uint64_t));

    sort(array_cpy, 0, batch_runs - 1);

    memset(&agg, 0, sizeof(metric_agg_t));
    agg.min = array_cpy[0];
    agg.max = array_cpy[batch_runs - 1];
    agg.median = array_cpy[(batch_runs - 1) / 2]; // lower median
    //agg.median = array_cpy[batch_runs / 2]; // upper median

    return agg;
}

void run_bench_1(void)
{

    batch_t batch;
    int batch_runs;
    int event_group[3] = {
        METRIC_CPU_CYCLES, METRIC_L1_CACHE_MISSES, METRIC_INSTRUCTIONS
    };
    metric_agg_t agg;

    batch_runs = 3;
    batch = batch_init(2, batch_runs, 3, event_group);

    init_contiguous_array();
    bench_perf_event(&batch, test_contiguous_array);
    clean_contiguous_array();

    for (int evt_idx = 0; evt_idx < batch.event_group_size; evt_idx++) {
        int event = batch.event_group[evt_idx];
        agg = metric_agg(batch.results[event], batch.batch_runs);
        printf("\n");
        printf("%s\n", metric_names[event]);
        printf("min:    %ld\n", agg.min);
        printf("max:    %ld\n", agg.max);
        printf("median: %ld\n", agg.median);
        printf("\n");
    }

    /*
    init_scattered_array();
    bench_perf_event(&batch, test_scattered_array);
    clean_scattered_array();

    printf("\n\n");
    printf("CPU_CYCLES:      %ld\n", batch.results[METRIC_CPU_CYCLES][0]);
    printf("INSTRUCTIONS:    %ld\n", batch.results[METRIC_INSTRUCTIONS][0]);
    printf("L1_CACHE_MISSES: %ld\n", batch.results[METRIC_L1_CACHE_MISSES][0]);
    printf("\n\n");
    */
}

