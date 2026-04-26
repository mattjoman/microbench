# Workloads

**Cyclops** provides a simple workload API, making it easy to create custom
workloads or experiment with one of the default workloads.

Workloads can be fully scriptable from the command line using custom workload
parameters.

To create a custom workload, you must make a workload C file in `/workloads`.
This C file will need to contain the following things:

- `#include "../include/workload.h"`
- declare a `static workload_t` struct
- define the following functions
    - `init()` - initialises workload state
    - `workload()` - the workload to benchmark
    - `clean()` - clean up any dynamically allocated state
- optionally declare a parameter array (so you can script the workload from the
  command line)
- register the workload with the `REGISTER_WORKLOAD()` macro

For examples see `/include/workload.h` and the default workloads in
`/workloads`.
