#include <stdio.h>
#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);

    printf("%lu\n", x);
}

void run_test_cache_miss() {
    uint64_t count_1 = bench_cache_miss(test_contiguous_array);
    uint64_t count_2 = bench_cache_miss(test_scattered_array);

    printf("\nL1 CACHE MISSES\n\n");
    printf("Array of integers vs array of pointers to heap integers\n\n");
    printf("Contiguous: %lu\nHeap: %lu\n\n", count_1, count_2);
}

