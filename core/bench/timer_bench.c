#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "../../include/bench.h"
#include "./internal.h"

#if defined(__x86_64__) || defined(__amd64__)

static uint64_t rdtscp()
{
    uint32_t lo, hi;
    __asm__ volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "ecx");
    return ((uint64_t)hi << 32) | lo;
}

static void bench_rdtscp(batch_conf_t *batch_cfg,
                 timer_batch_t *batch_data,
                 void (*workload)(void))
{
    uint64_t start, end;

    pin_thread();

    for (unsigned long long i = 0; i < batch_cfg->warmup_runs; i++) {
        workload();
    }

    for (unsigned long long i = 0; i < batch_cfg->batch_runs; i++) {
        start = rdtscp();
        workload();
        end = rdtscp();
        batch_data->timer.run_vals[i] = end - start;
    }
}

#endif

bench_func_t get_timer_bench_func(mg_id_t id)
{
    switch (id) {

        case MG_ID_RDTSCP:
#if defined(__x86_64__) || defined(__amd64__)
            return bench_rdtscp;
#else
            fprintf(stderr, "RDTSCP benchmark is x86-specific\n");
            exit(1);
#endif

        default:
            exit(1);
    }
}
