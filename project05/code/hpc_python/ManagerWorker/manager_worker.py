from mandelbrot_task import *
import matplotlib as mpl

mpl.use("Agg")
import matplotlib.pyplot as plt
from mpi4py import MPI  # MPI_Init and MPI_Finalize automatically called
import numpy as np
import sys
import time

# some parameters
MANAGER = 0  # rank of manager
TAG_TASK = 1  # task       message tag
TAG_TASK_DONE = 2  # tasks done message tag
TAG_DONE = 3  # done       message tag


def manager(comm, tasks):
    """
    The manager.

    Parameters
    ----------
    comm : mpi4py.MPI communicator
        MPI communicator
    tasks : list of objects with a do_task() method perfroming the task
        List of tasks to accomplish

    Returns
    -------
    ... ToDo ...
    """
    num_workers = comm.Get_size()
    queue = tasks[:]
    task_status = {i: None for i in range(1, num_workers)}
    completed_tasks = []
    task_counter = [0] * num_workers
    

    # Initially distribute tasks to all workers
    for worker in range(1, num_workers):
        if queue:
            task = queue.pop(0)
            # print(f"[MANAGER] SEND INITIAL TASK to WORKER={worker} of type {type(task)}", flush=True)
            comm.send(task, dest=worker, tag=TAG_TASK)
            task_status[worker] = task
        else:
            break

    while len(queue) > 0 or any(task_status.values()):
        # Receive results from workers
        status = MPI.Status()
        result = comm.recv(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=status)
        worker = status.Get_source()  # get rank from sending worker
        tag = status.Get_tag()
        if tag == TAG_TASK_DONE:
            completed_tasks.append(result)
            task_counter[worker] += 1
            # print(f"[MANAGER]: Result = {result}")
        task_status[worker] = None

        # If tasks still available, send it to worker
        if queue:
            task = queue.pop(0)
            comm.send(task, dest=worker, tag=TAG_TASK)
            # print(f"[MANAGER] REISSUED TASK to WORKER={worker}", flush=True)
            task_status[worker] = task
        # Otherwise send TAG_DONE to the worker
        else:
            comm.send(None, dest=worker, tag=TAG_DONE)
            # print(f"[MANAGER] Send DONE to WORKER={worker}", flush=True)

    # print("MANAGER DONE")
    return completed_tasks, task_counter


def worker(comm):
    """
    The worker.

    Parameters
    ----------
    comm : mpi4py.MPI communicator
        MPI communicator
    """
    while True:
        status = MPI.Status()
        task = comm.recv(source=MANAGER, tag=MPI.ANY_TAG, status=status)
        tag = status.Get_tag()
        # print(f"[{comm.Get_rank()}] GOT TASK FROM {MANAGER} with tag {tag}", flush=True)

        if tag == TAG_TASK:
            task.do_work()
            comm.send(task, dest=MANAGER, tag=TAG_TASK_DONE)
            # print(f"[{comm.Get_rank()}] SEND RESULT FROM WORKER to MANAGER with tag {tag}", flush=True)
        elif tag == TAG_DONE:
            # print(f"[{comm.Get_rank()}] RECEIVED DONE", flush=True)
            break
    # print(f"[{comm.Get_rank()}] Worker DONE", flush=True)


def readcmdline(rank):
    """
    Read command line arguments

    Parameters
    ----------
    rank : int
        Rank of calling MPI process

    Returns
    -------
    nx : int
        number of gridpoints in x-direction
    ny : int
        number of gridpoints in y-direction
    ntasks : int
        number of tasks
    """
    # report usage
    if len(sys.argv) != 4:
        if rank == MANAGER:
            print("Usage: manager_worker nx ny ntasks")
            print("  nx     number of gridpoints in x-direction")
            print("  ny     number of gridpoints in y-direction")
            print("  ntasks number of tasks")
        MPI.Finalize()
        sys.exit()

    # read nx, ny, ntasks
    nx = int(sys.argv[1])
    if nx < 1:
        MPI.Finalize()
        sys.exit("nx must be a positive integer")
    ny = int(sys.argv[2])
    if ny < 1:
        MPI.Finalize()
        sys.exit("ny must be a positive integer")
    ntasks = int(sys.argv[3])
    if ntasks < 1:
        MPI.Finalize()
        sys.exit("ntasks must be a positive integer")

    return nx, ny, ntasks


if __name__ == "__main__":

    # get COMMON WORLD communicator, size & rank
    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    my_rank = comm.Get_rank()

    # report on MPI environment
    if my_rank == MANAGER:
        print(f"MPI initialized with {size:5d} processes", flush=True)

        # read command line arguments
        nx, ny, ntasks = readcmdline(my_rank)

        # start timer
        timespent = -time.perf_counter()

        # trying out ... YOUR MANAGER-WORKER IMPLEMENTATION HERE ...
        x_min = -2.0
        x_max = +1.0
        y_min = -1.5
        y_max = +1.5
        M = mandelbrot(x_min, x_max, nx, y_min, y_max, ny, ntasks)
        tasks = M.get_tasks()

        results, tasksDoneByWorker = manager(comm, tasks)
        # print("GATHERED all RESULTS", flush=True)
        m = M.combine_tasks(results)
        plt.imshow(m.T, cmap="gray", extent=[x_min, x_max, y_min, y_max])
        plt.savefig("mandelbrot.png")

        # stop timer
        timespent += time.perf_counter()
    else:
        worker(comm)

    # inform that done
    if my_rank == MANAGER:
        print(f"Run took {timespent:f} seconds")
        for i in range(size):
            if i == MANAGER:
                continue
            print(f"Process {i:5d} has done {tasksDoneByWorker[i]:10d} tasks")
        print("Done.")
