# Compiler and flags
CC=gcc
CFLAGS=-Iinclude -O0 -Wall -Wextra -std=gnu11 -g

# Source files
SRCS=src/main.c \
		 src/bench.c \
		 src/workload.c \
		 src/batch.c \
		 src/analysis.c \
		 src/counter.c \
		 src/report.c

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

