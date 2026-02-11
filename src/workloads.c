#include <stdlib.h>
#include <stdio.h>

#include "../include/include.h"

//#define BIG_NUMBER_1 777777
#define BIG_NUMBER_1 10000000

int contiguous_array[BIG_NUMBER_1];

static void init_contiguous_array()
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        contiguous_array[i] = i;
    }
}

static void clean_contiguous_array(void)
{
    return;
}

__attribute__((noinline))
static void contiguous_array_func(void)
{
    volatile int sum;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += contiguous_array[i];
    }
}

int *scattered_array[BIG_NUMBER_1];

static void init_scattered_array(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        scattered_array[i] = malloc(sizeof(int));
        *scattered_array[i] = i;
    }
}

static void clean_scattered_array(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        free(scattered_array[i]);
    }
}

__attribute__((noinline))
static void scattered_array_func(void)
{
    volatile int sum;

    sum = 0;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += *scattered_array[i];
    }
}

static const workload_t wl_contiguous_array = {
    .init = init_contiguous_array,
    .clean = clean_contiguous_array,
    .workload = contiguous_array_func,
};

static const workload_t wl_scattered_array = {
    .init = init_scattered_array,
    .clean = clean_scattered_array,
    .workload = scattered_array_func,
};

const workload_t *get_workload(int workload)
{
    switch (workload) {
        case WL_CONTIGUOUS_ARRAY:
            return &wl_contiguous_array;
        case WL_SCATTERED_ARRAY:
            return &wl_scattered_array;
        default:
            break;
    }

    return NULL;
}
