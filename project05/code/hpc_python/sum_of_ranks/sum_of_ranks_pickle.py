from mpi4py import MPI
import numpy as np

# get comm, size, rank & host name
comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()
proc = MPI.Get_processor_name()

sum = np.array(rank, dtype=np.int32)
recv_rank = np.array(rank, dtype=np.int32)
send_rank = np.array(rank, dtype=np.int32)

for _ in range(size -1):
    if rank % 2 == 0:
        comm.send(send_rank, dest=(rank + 1) %size, tag=0)
        recv_rank = comm.recv(source=(rank - 1 + size) %size, tag=0)
    else: 
        recv_rank = comm.recv(source=(rank - 1 + size) %size, tag=0)
        comm.send(send_rank, dest=(rank + 1) %size, tag=0)

    sum += recv_rank
    send_rank = recv_rank

print(f"Process {rank} -> sum: {sum}")
