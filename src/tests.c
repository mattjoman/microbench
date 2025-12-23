#include <stdlib.h>
#include <stdio.h>

#include "../include/include.h"

void test_loop() {
    volatile long long x = 0; // volatile so the compiler doesn't optimise it away
    while (x < 1000000) {
        x++;
    }
    return;
}

int contiguous_array[BIG_NUMBER_1];

void init_contiguous_array() {
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        contiguous_array[i] = i;
    }
}

void clean_contiguous_array() {
    return;
}

void test_contiguous_array() {
    volatile int sum;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += contiguous_array[i];
    }
}

int *scattered_array[BIG_NUMBER_1];

void init_scattered_array() {
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        scattered_array[i] = malloc(sizeof(int));
        *scattered_array[i] = i;
    }
}

void clean_scattered_array() {
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        free(scattered_array[i]);
    }
}

void test_scattered_array() {
    volatile int sum;

    sum = 0;
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += *scattered_array[i];
    }
}

