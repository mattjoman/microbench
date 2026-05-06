import matplotlib.pyplot as plt
import pandas as pd

from cyclops import Cyclops, ParamSweep

BATCH_RUNS = 1
WARMUP_RUNS = 0

#AGGREGATE = "MIN"
#AGGREGATE = "MAX"
AGGREGATE = "MEDIAN"

WORKLOAD = "BRANCH"

METRIC_GRP = "BRANCH"
METRIC = "BRANCH_MISPRED_RATE"
#METRIC_GRP = "BPU_READS"
#METRIC = "BPU_READ_MISS_RATE"

FILE_NAME = "branch.csv"

def sweep_pattern_len(bias: int):

    param_sweep = ParamSweep(
        key="pattern-len",
        low=10,
        high=1000,
        step=10,
    )

    cyclops = Cyclops(
        workload=WORKLOAD,
        metric_grp=METRIC_GRP,
        warmup_runs=WARMUP_RUNS,
        batch_runs=BATCH_RUNS,
        params={
            "n-branches": 100000,
            "bias": bias,
        },
        param_sweep=param_sweep,
        file_name=FILE_NAME,
    )
    cyclops.exec()

    df = pd.read_csv(
        f"{FILE_NAME}",
        comment="#",
        index_col=param_sweep.key
    )

    return df.index.values, df[f"{METRIC}:{AGGREGATE}"].values

def run_bias_experiment(bias_take: bool):

    if bias_take:
        biases = [50, 60, 70, 80, 90, 95, 99]
        file_name = "branch_bias_take.png"
        figure_title = "Misprediction Rate (bias: take branch)"
    else:
        biases = [50, 40, 30, 20, 10, 5, 1]
        file_name = "branch_bias_dont_take.png"
        figure_title = "Misprediction Rate (bias: don't take branch)"

    data = []
    for bias in biases:
        x, y = sweep_pattern_len(bias)
        data.append({ "x": x, "y": y, "bias": bias})

    plt.figure()

    for d in data:
        plt.plot(d["x"], d["y"], marker="", label=f"Bias: {d["bias"]}")

    #plt.xscale("log")
    #plt.yscale("log")
    plt.xlabel("Pattern Lenth")
    plt.ylabel(METRIC)
    plt.title(figure_title)
    plt.grid(True)
    plt.ylim(0, 0.3)
    plt.legend(loc='center right', bbox_to_anchor=(1, 0.5))
    plt.savefig(file_name)
    plt.close()

if __name__ == "__main__":
    run_bias_experiment(True)
    run_bias_experiment(False)
