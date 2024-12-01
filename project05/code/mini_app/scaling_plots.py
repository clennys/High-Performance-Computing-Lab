import matplotlib.pyplot as plt
import pandas as pd

def strong_scaling_plot(parallel_times, array_sizes, num_threads):
    plt.figure(figsize=(10, 6))

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], parallel_times[i], marker='o', label=f"N = {array_size}")

    plt.title(f"Strong Scaling Mini-App using MPI")
    plt.xlabel("Number of Processes")
    plt.ylabel("Execution Time [log(s)]")
    plt.grid(True)
    plt.legend()
    plt.yscale("log")
    plt.xticks(num_threads)

    plt.savefig(f"strong_scaling.png")

    # plt.show()

def weak_scaling_plot(parallel_times, array_sizes, num_threads):
    plt.figure(figsize=(10, 6))
    colors = ['tab:blue', 'tab:orange', 'tab:green']

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], parallel_times[i], marker='o', color=colors[i], label=f"Base Res = {array_size}")
        ideal_time = parallel_times[i][0]
        plt.plot(num_threads, [ideal_time] * len(num_threads), linestyle='--', color=colors[i], label=f"Optimal Weak Scaling ({array_size})")


    plt.title(f"Weak Scaling Mini-App using MPI")
    plt.xlabel("Number of Processes")
    plt.ylabel("Execution Time [log(s)]")
    plt.grid(True)
    plt.legend()
    plt.yscale("log")
    plt.xticks(num_threads)

    plt.savefig(f"weak_scaling.png")

    # plt.show()

df = pd.read_csv('pde.csv', sep=',')
df_strong = df.iloc[:25, :]
df_weak = df.iloc[25:, :]
print(df_strong)
print(df_weak)
res = df_strong["resolution"].unique()
num_threads = df_strong["Processes"].unique()

parallel_times = []
for r in res:
    parallel_times.append(df_strong[df_strong["resolution"] == r]["time"].tolist())

strong_scaling_plot(parallel_times, res, num_threads)

base_res = ["64x64", "128x128", "256x256"]

df_64 = df_weak.iloc[:5, :]
df_128 = df_weak.iloc[5:10, :]
df_256 = df_weak.iloc[10:, :]

weak_parallel_times = [
        df_64["time"].tolist(),
        df_128["time"].tolist(),
        df_256["time"].tolist()
        ]

weak_scaling_plot(weak_parallel_times, base_res, num_threads)
