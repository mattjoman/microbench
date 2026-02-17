# TODO

## Cli

```bash
./mb --workload 2 --egroup 1
```

## Naming

## Pre data processing

- Counters refer to the raw results from `perf` or `rdtscp`

## Post data processing

- Ratios refer to the ratios calculated from counters
- Metrics refer to both counters and ratios

## Data Processing

In `analysis_t`:

- Change `aggregates` to `metrics`
- Add un-aggregated data arrays to each metric
