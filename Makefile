# Compiler and flags
CC=gcc

# Build modes
PROD_CFLAGS = -O3 -g -Wall -Wextra -Wpedantic -std=gnu11
DEBUG_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11
ASAN_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11 -fsanitize=address
UBSAN_CFLAGS = -O0 -g -Wall -Wextra -Wpedantic -std=gnu11 \
	-fsanitize=undefined -fsanitize=float-divide-by-zero

CFLAGS ?= $(PROD_CFLAGS)

CORE_SRCS=core/main.c \
	core/metric_grp/frontend.c \
	core/metric_grp/backend.c \
	core/metric_grp/backend_common.c \
	core/metric_grp/backend_perf.c \
	core/metric_grp/backend_cpu_instruction.c \
	core/metric_grp/metric_grps/alignment_faults.c \
	core/metric_grp/metric_grps/arm_timer.c \
	core/metric_grp/metric_grps/bpu_reads.c \
	core/metric_grp/metric_grps/branch.c \
	core/metric_grp/metric_grps/dtlb_reads.c \
	core/metric_grp/metric_grps/ipc.c \
	core/metric_grp/metric_grps/itlb_reads.c \
	core/metric_grp/metric_grps/l1d_reads.c \
	core/metric_grp/metric_grps/l1i_reads.c \
	core/metric_grp/metric_grps/llc_reads.c \
	core/metric_grp/metric_grps/page_faults.c \
	core/metric_grp/metric_grps/rdtscp.c \
	core/metric_grp/metric_grps/stalled_cycles.c \
	core/experiment/batch.c \
	core/experiment/param_sweep.c \
	core/experiment/report.c \
	core/experiment/data_processing.c \
	core/workload.c

WORKLOAD_SRCS := $(wildcard workloads/*.c)

SRCS := $(CORE_SRCS) $(WORKLOAD_SRCS)
OBJS=$(SRCS:.c=.o)

# Output binary
OUT=cyclops

# Default target
all: $(OUT)

# Debug target overrides CFLAGS
debug: CFLAGS := $(DEBUG_CFLAGS)
debug: clean $(OUT)

# Asan target overrides CFLAGS
asan: CFLAGS := $(ASAN_CFLAGS)
asan: clean $(OUT)

# Asan target overrides CFLAGS
ubsan: CFLAGS := $(UBSAN_CFLAGS)
ubsan: clean $(OUT)

# Link objects
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile sources
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

clear:
	rm -f $(OBJS) $(OUT)

.PHONY: all clean clear
