#include <stdlib.h>

#include "../include/workload.h"

int *scattered_array[BIG_NUMBER_1];

static void init(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        scattered_array[i] = malloc(sizeof(int));
        *scattered_array[i] = i;
    }
}

static void clean(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        free(scattered_array[i]);
    }
}

__attribute__((noinline))
static void workload(void)
{
    volatile int sum;

    sum = 0;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += *scattered_array[i];
    }
}

workload_t wl_scattered_array = {
    .id = WL_SCATTERED_ARRAY,
    .name = "SCATTERED_ARRAY",
    .init = init,
    .clean = clean,
    .workload = workload,
};
