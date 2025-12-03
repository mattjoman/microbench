#include <stdlib.h>
#include "../include/include.h"




#define BIG_NUMBER_1 1000




void test_loop() {
    volatile long long x = 0; // volatile so the compiler doesn't optimise it away
    while (x < 1000000) {
        x++;
    }
    return;
}




void test_contiguous_array() {
    int arr[BIG_NUMBER_1];
    volatile int sum;

    for (int i = 0; i < BIG_NUMBER_1; i++) {
        arr[i] = i;
    }
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += arr[i];
    }
}

void test_scattered_array() {
    int *arr[BIG_NUMBER_1];
    volatile int sum = 0;

    for (int i = 0; i < BIG_NUMBER_1; i++) {
        arr[i] = malloc(sizeof(int));
        *arr[i] = i;
    }
    for (int i = 0; i < BIG_NUMBER_1; i++) {
        sum += *arr[i];
        free(arr[i]);
    }
}

