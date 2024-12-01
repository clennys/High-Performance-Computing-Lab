import matplotlib.pyplot as plt
import pandas as pd

def strong_scaling_plot(parallel_times, array_sizes, num_threads):
    plt.figure(figsize=(10, 6))

    for i, array_size in enumerate(array_sizes):
        plt.plot(num_threads[:len(parallel_times[i])], parallel_times[i], marker='o', label=f"Total Tasks = {array_size}")

    plt.title(f"Self-scheduling Parallel Mandelbrot")
    plt.xlabel("Number of Processes")
    plt.ylabel("Execution Time [s]")
    plt.grid(True)
    plt.legend()
    # plt.yscale("log")
    plt.xticks(num_threads)

    plt.savefig(f"manager-worker.png")

    plt.show()


df = pd.read_csv('output.csv', sep=',')
print(df)
tasks = df["Total Tasks"].unique()
num_threads = df["Processes"].unique()

parallel_times = []
for t in tasks:
    parallel_times.append(df[df["Total Tasks"] == t]["Execution Time"].tolist())

strong_scaling_plot(parallel_times, tasks, num_threads)
