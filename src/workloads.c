#include <stdlib.h>
#include <stdio.h>

#include "../include/include.h"

#define BIG_NUMBER_1 10000

int contiguous_array[BIG_NUMBER_1];

void init_contiguous_array()
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        contiguous_array[i] = i;
    }
}

void clean_contiguous_array(void)
{
    return;
}

void test_contiguous_array(void)
{
    volatile int sum;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += contiguous_array[i];
    }
}

int *scattered_array[BIG_NUMBER_1];

void init_scattered_array(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        scattered_array[i] = malloc(sizeof(int));
        *scattered_array[i] = i;
    }
}

void clean_scattered_array(void)
{
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        free(scattered_array[i]);
    }
}

void test_scattered_array(void)
{
    volatile int sum;

    sum = 0;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += *scattered_array[i];
    }
}
