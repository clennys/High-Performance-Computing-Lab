#include <mpi.h> // MPI
#include <stdio.h>

int main(int argc, char *argv[]) {

  // Initialize MPI, get size and rank
  int size, rank, tag = 100;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // IMPLEMENT: Ring sum algorithm
  int sum = rank;
  int rec_rank = rank;
  int send_rank = rank;
  for (int i = 0; i < size - 1; i++) {
    if (rank % 2 == 0) {
      MPI_Send(&send_rank, 1, MPI_INT, (rank + 1) % size, tag, MPI_COMM_WORLD);
      MPI_Recv(&rec_rank, 1, MPI_INT, (rank - 1 + size) % size, tag,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(&rec_rank, 1, MPI_INT, (rank - 1 + size) % size, tag,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&send_rank, 1, MPI_INT, (rank + 1) % size, tag, MPI_COMM_WORLD);
    }
    sum += rec_rank;
    send_rank = rec_rank;

    // printf("Iter %i -> Process %i: Sum = %i\n", i, rank, sum);
  }
  printf("Process %i: Sum = %i\n", rank, sum);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
