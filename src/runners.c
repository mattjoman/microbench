#include <stdio.h>
#include "../include/include.h"

void run_rdtscp_test_loop() {
    uint64_t x = bench_rdtscp(test_loop);
    printf("%lu\n", x);
}

