#include "../include/include.h"

void test_loop() {
    volatile long long x = 0; // volatile so the compiler doesn't optimise it away
    while (x < 1000000) {
        x++;
    }
    return;
}

