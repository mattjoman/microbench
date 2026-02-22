#include "../include/workload.h"

int contiguous_array[BIG_NUMBER_1];

static void init(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        contiguous_array[i] = i;
    }
}

static void clean(void)
{
    return;
}

__attribute__((noinline))
static void workload(void)
{
    volatile int sum;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += contiguous_array[i];
    }
}

workload_t wl_contiguous_array = {
    .id = WL_CONTIGUOUS_ARRAY,
    .name = "CONTIGUOUS_ARRAY",
    .init = init,
    .clean = clean,
    .workload = workload,
};
