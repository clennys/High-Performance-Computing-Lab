from mpi4py import MPI
import numpy as np

SUBDOMAIN = 6
DOMAINSIZE = SUBDOMAIN + 2


def print_grid(data):
    for i in range(DOMAINSIZE):
        row = [f"{data[j + i * DOMAINSIZE]:4.1f}" for j in range(DOMAINSIZE)]
        print(" ".join(row))


comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()
proc = MPI.Get_processor_name()

if size != 16:
    print("Run this with 16 processors")
    MPI.Finalize()
    exit()

data = np.full((DOMAINSIZE * DOMAINSIZE), rank, dtype=np.float64)

dims = [4, 4]
periods = [True, True]

comm_cart = comm.Create_cart(dims, periods, False)
coords = comm_cart.Get_coords(rank)
print(f"Rank {rank} coords ({coords[0]}, {coords[1]})")

rank_top, rank_bottom = comm_cart.Shift(0, 1)
rank_left, rank_right = comm_cart.Shift(1, 1)

top_right_rank = comm_cart.Get_cart_rank([coords[0] - 1, coords[1] + 1])
top_left_rank = comm_cart.Get_cart_rank([coords[0] - 1, coords[1] - 1])
bottom_right_rank = comm_cart.Get_cart_rank([coords[0] + 1, coords[1] + 1])
bottom_left_rank = comm_cart.Get_cart_rank([coords[0] + 1, coords[1] - 1])


T_col = MPI.DOUBLE.Create_vector(SUBDOMAIN, 1, DOMAINSIZE)
T_col.Commit()

# S:Top/R:Bottom
recv_temp = np.empty(SUBDOMAIN, dtype=np.float64)
domain_send = DOMAINSIZE + 1
domain_recv = (SUBDOMAIN + 1) * DOMAINSIZE + 1
comm_cart.Sendrecv(
    data[domain_send : domain_send + SUBDOMAIN],
    dest=rank_top,
    sendtag=0,
    recvbuf=recv_temp,
    source=rank_bottom,
    recvtag=0,
)

data[domain_recv : domain_recv + SUBDOMAIN] = recv_temp

# S:Bottom/R:Top
recv_temp_b = np.empty(SUBDOMAIN, dtype=np.float64)
domain_send = SUBDOMAIN * DOMAINSIZE + 1
domain_recv = 1
comm_cart.Sendrecv(
    data[domain_send : domain_send + SUBDOMAIN],
    dest=rank_bottom,
    sendtag=0,
    recvbuf=recv_temp,
    source=rank_top,
    recvtag=0,
)

data[domain_recv : domain_recv + SUBDOMAIN] = recv_temp

# S:left/R:right
recv_temp = np.empty(SUBDOMAIN, dtype=np.float64)
domain_send = DOMAINSIZE + 1
domain_recv = DOMAINSIZE + (SUBDOMAIN + 1)
comm_cart.Sendrecv(
    [data[domain_send:], 1, T_col],
    dest=rank_left,
    sendtag=0,
    recvbuf=[recv_temp, MPI.DOUBLE],
    source=rank_right,
    recvtag=0,
)

data[domain_recv : domain_recv + SUBDOMAIN * DOMAINSIZE : DOMAINSIZE] = recv_temp

# S:right/R:left
recv_temp = np.empty(SUBDOMAIN, dtype=np.float64)
domain_send = DOMAINSIZE + SUBDOMAIN
domain_recv = DOMAINSIZE
comm_cart.Sendrecv(
    [data[domain_send:], 1, T_col],
    dest=rank_right,
    sendtag=0,
    recvbuf=[recv_temp, MPI.DOUBLE],
    source=rank_left,
    recvtag=0,
)

data[domain_recv : domain_recv + SUBDOMAIN * DOMAINSIZE : DOMAINSIZE] = recv_temp


if rank == 9:
    print("Data of rank 9 after communication:")
    print_grid(data)

T_col.free()
comm_cart.Free()
