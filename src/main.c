#include <stdio.h>
#include "../include/include.h"

int main() {
    uint64_t start, end;
    start = timecheck();
    
    volatile long long x = 0; // volatile so the compiler doesn't optimise it away
    while (x < 100000000) {
        x++;
    }
    
    end = timecheck();
    printf("Loops: %llu, Cycles = %llu\n", x, (unsigned long long)(end - start));
}

