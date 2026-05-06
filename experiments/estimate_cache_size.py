import matplotlib.pyplot as plt
import pandas as pd

from cyclops import Cyclops, ParamSweep

BATCH_RUNS = 50
WARMUP_RUNS = 5

#AGGREGATE = "MIN"
#AGGREGATE = "MAX"
AGGREGATE = "MEDIAN"

WORKLOAD = "STRIDED_ARRAY"

FILE_NAME = "estimate_cache_size.csv"

L1D = 0
LLC = 1

def sweep_array_elements(cache: int):

    if cache == L1D:
        array_elements_low = 100
        array_elements_high = 1000
        array_elements_step = 10
        metric_grp = "L1D_READS"
        metric = "L1D_READ_MISS_RATE"
    elif cache == LLC:
        array_elements_low = 1000
        array_elements_high = 10000
        array_elements_step = 1000
        metric_grp = "LLC_READS"
        metric = "LLC_READ_MISS_RATE"

    param_sweep = ParamSweep(
        key="array-elements",
        low=array_elements_low,
        high=array_elements_high,
        step=array_elements_step,
    )

    cyclops = Cyclops(
        workload=WORKLOAD,
        metric_grp=metric_grp,
        warmup_runs=WARMUP_RUNS,
        batch_runs=BATCH_RUNS,
        param_sweep=param_sweep,
        file_name=FILE_NAME,
    )
    cyclops.exec()

    df = pd.read_csv(
        f"{FILE_NAME}",
        comment="#",
        index_col=param_sweep.key
    )

    return 64 * df.index.values, df[f"{metric}:{AGGREGATE}"].values

if __name__ == "__main__":

    x_L1D, y_L1D = sweep_array_elements(L1D)
    x_LLC, y_LLC = sweep_array_elements(LLC)

    plt.figure()
    plt.plot(x_L1D, y_L1D, marker="", label="L1D")
    plt.plot(x_LLC, y_LLC, marker="", label="LLC")
    plt.xscale("log")
    #plt.yscale("log")
    plt.xlabel("Array size (Bytes)")
    plt.ylabel("Cache miss rate")
    plt.title("Cache Miss Rate vs Array Size")
    plt.grid(True)
    plt.ylim(0, 1)
    plt.legend()
    plt.savefig(f"estimate_cache_size.png")
    plt.close()
