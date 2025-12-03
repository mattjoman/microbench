#include "../include/include.h"




/*** STATIC HELPERS ***/

static uint64_t rdtscp() {
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}




/*** BENCHMARKS ***/

uint64_t bench_rdtscp(void (*f)(void)) {
    uint64_t start, end;
    start = rdtscp();
    f();
    end = rdtscp();
    return end - start;
}

