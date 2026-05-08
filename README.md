# Cyclops

## Project Overview

**Cyclops** is a framework for investigating CPU behaviour/architecture and
custom code performance.

It allows the user to create experiments using low-level resources like PMU
counters and `rdtscp`, with sensible implementations to improve avoid pitfalls.
**Cyclops** makes it easy to design reproducible and accurate experiments that
can be easily orchestrated from the command line and visualised to gain
insights.

## Features

- Batches with user-defined warmup runs, batch runs and aggregation
- Workload plugin system to easily write custom workloads with scriptable
  workload parameters (see `docs/workload.md`)
- Parameter sweeps to run multiple batches while varying one parameter and
  keeping the others fixed
- Metric groups including raw metrics like PMU counters (`perf_event_open()`)
  and `rdtscp`, and ratios like IPC (see `docs/metrics.md` for measurement
  methodology)
- Results are written to stdout & CSV files with metadata for reproducibility
- Python scripts for running and visualising some default experiments - see
  `experiments/`

## Build and Run

```bash
# clone repository
git clone https://mj-penney/cyclops.git cyclops

# enter repository
cd cyclops

# build
make

# run (generate help text)
./cyclops -h
```

## Example Usage

### Example 1

- Single batch
- 10 warmup runs
- 20 batch runs
- `BRANCH` workload with default params
- `IPC` metric group

```bash
./cyclops -u 10 -r 20 -w BRANCH -m IPC
```

### Example 2

- 3 warmup runs
- 5 batch runs
- `STRIDED_ARRAY` workload
- `L1D_READS` metric group
- 19 batches, sweeping the `array-elements` param from 10 to 100 in steps of 5

```bash
./cyclops -u 3 -r 5 -w STRIDED_ARRAY -m L1D_READS -s array-elements=10:100:5
```

## Experiments

The `cyclops` tool is designed to be highly scriptable, and make it easy to
design performance & microarchitecture experiments.

In `experiments/` there are example Python scripts for running experiments.
To run these experiments, you will first need to build the `cyclops` binary
(see above).

You will then need to create a Python virtual environment and install the
necessary packages:

```bash
cd experiments
python -m venv venv
pip install -r requirements.txt
```

Before running the example experiments, activate the virtual environment you
just created:

```bash
source venv/bin/activate
```

Then run the experiments:

```bash
python3 estimate_cache_size.py
python3 branch.py
```

Figures (png) will be saved in `experiments/`.

See `investigations/` for investigations and writeups using **Cyclops**:

- ![Reverse-engineering my cache](investigations/estimate_cache_size.md)
