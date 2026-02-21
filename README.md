# Microbench

A low-overhead microbenchmarking tool for Linux build directly on top of
`perf_event_open()`.

## Motivation

Modern performance engineering requires understanding how high-level code maps
to low-level hardware behavior.
I built **Microbench** to:

- Develop intuition for how design decisions affect CPU performance
- Learn how to diagnose bottlenecks using hardware performance counters
- Explore the challenges of accurate microbenchmarking
- Understand the tradeoffs between measurement accuracy and measurement
  overhead

While tools like perf are powerful, they are general-purpose.
I wanted a minimal tool tailored specifically for controlled, repeatable
microbenchmarks.

## Microbenchmarking Challenges

Microbenchmarking on modern CPUs is deceptively difficult due to:

- Counter multiplexing when too many events are measured simultaneously
- Scheduler noise and thread migration
- Cold instruction/data caches
- Branch predictor state
- Measurement overhead influencing results
- Variance across runs due to microarchitectural state

Microbench was designed to explicitly address these issues.

## Approach

To improve measurement fidelity, the tool uses the following strategies:

- **Limited counter group size (max 3 events):**
  Prevents kernel multiplexing and avoids scaled counter results.

- **Thread pinning:**
  Pins the benchmarking thread to a single core to eliminate migration noise.

- **Warmup runs:**
  Warms instruction/data caches and branch predictors before measurement.

- **Precise counter scoping**
  Enables the counter group immediately before the workload and disables it
  immediately after to minimize measurement pollution.

- **Result buffering:**
  Defers processing and aggregation to avoid perturbing the microarchitectural
  state of subsequent runs.

- **Per-run derived metrics:**
  Calculates ratios (e.g., IPC = instructions / cycles) per run before
  aggregation to avoid statistical distortion.
