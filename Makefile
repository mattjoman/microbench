# Compiler and flags
CC=gcc
CFLAGS=-Iinclude -O0 -Wall -Wextra -std=gnu11 -g

# Source files
SRCS=src/main.c \
		 src/benchmarks.c \
		 src/workloads.c \
		 src/experiments.c \
		 src/analysis.c \
		 src/metrics.c

OBJS=$(SRCS:.c=.o)

# Output binary
OUT=out

# Default target
all: $(OUT)

# Link objects
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile sources
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(OUT)

.PHONY: all clean

