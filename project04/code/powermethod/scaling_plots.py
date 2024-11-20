import matplotlib.pyplot as plt
import pandas as pd

def strong_scaling_plot(parallel_times, array_sizes, num_threads):
    plt.figure(figsize=(10, 6))

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], parallel_times[i], marker='o', label=f"{array_size}")

    plt.title(f"Strong Scaling Power Method")
    plt.xlabel("Number of MPI Processes")
    plt.ylabel("Execution Time [s]")
    plt.grid(True)
    plt.legend()
    # plt.yscale("log")
    plt.xticks(num_threads)

    plt.savefig(f"strong_scaling.png")

    # plt.show()

def weak_scaling_plot(parallel_times, array_sizes, num_threads):
    plt.figure(figsize=(10, 6))

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], parallel_times[i], marker='o', label=f"{array_size}")

    plt.title(f"Weak Scaling Power Method with Base Resolution 10'000x10'000")
    plt.xlabel("Number of MPI Processes")
    plt.ylabel("Execution Time [s]")
    plt.grid(True)
    plt.legend()
    # plt.yscale("log")
    plt.xticks(num_threads)

    plt.savefig(f"weak_scaling.png")
    # plt.show()

def plot_efficiency_strong(serial_times, parallel_times, array_sizes, name):

    efficiency_data = []
    for i in range(len(array_sizes)):
        efficiency = [serial_times[i] / (parallel * threads) for parallel, threads in zip(parallel_times[i], num_threads)]
        efficiency_data.append(efficiency)

    plt.figure(figsize=(10, 6))

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], efficiency_data[i], marker='o', label=f"{array_size}")

    plt.axhline(y=1, color='red', linestyle='--', label='Optimal Efficiency')
    plt.title(f"Efficiency Plot ({name})")
    plt.xlabel("Number of MPI Processes")
    plt.ylabel("Efficiency")
    plt.ylim(0, 1.1)
    plt.grid(True)
    # plt.xscale("log")
    plt.xticks(num_threads, labels=[str(t) for t in num_threads])
    plt.legend()

    plt.savefig(f"efficiency_plot_eff_strong.png")

    # plt.show()

def plot_efficiency_weak(serial_times, parallel_times, array_sizes, name):

    efficiency_data = []
    for i in range(len(array_sizes)):
        efficiency = [serial_times[i] / parallel for parallel, threads in zip(parallel_times[i], num_threads)]
        efficiency_data.append(efficiency)

    plt.figure(figsize=(10, 6))

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], efficiency_data[i], marker='o', label=f"{array_size}")

    plt.axhline(y=1, color='red', linestyle='--', label='Optimal Efficiency')
    plt.title(f"Efficiency Plot ({name})")
    plt.xlabel("Number of MPI Processes")
    plt.ylabel("Efficiency")
    plt.ylim(0, 1.1)
    plt.grid(True)
    # plt.xscale("log")
    plt.xticks(num_threads, labels=[str(t) for t in num_threads])
    plt.legend()

    plt.savefig(f"efficiency_plot_eff_weak.png")

    # plt.show()

df = pd.read_csv('./powermethod.csv', sep=',')
df_strong = df.iloc[:5, :]
df_weak = df.iloc[5:, :]

df_all_nodes = pd.read_csv('./powermethod_all_nodes.csv', sep=',')
df_strong_all_nodes = df_all_nodes.iloc[:5, :]
df_weak_all_nodes = df_all_nodes.iloc[5:, :]

names = ["One Node for all Process", "One Node per Process"]
num_threads = df_strong["threads"].unique()

parallel_times = []
parallel_times.append(df_strong["time"].tolist())
parallel_times.append(df_strong_all_nodes["time"].tolist())

serial_times = [parallel_times[0][0], parallel_times[1][0]]
plot_efficiency_strong(serial_times, parallel_times, names, "Strong Scaling")

strong_scaling_plot(parallel_times, names, num_threads)

parallel_times_weak = []
parallel_times_weak.append(df_weak["time"].tolist())
parallel_times_weak.append(df_weak_all_nodes["time"].tolist())
serial_times_weak = [parallel_times_weak[0][0], parallel_times_weak[1][0]]

weak_scaling_plot(parallel_times_weak, names, num_threads)
plot_efficiency_weak(serial_times_weak, parallel_times_weak, names, "Weak Scaling")
