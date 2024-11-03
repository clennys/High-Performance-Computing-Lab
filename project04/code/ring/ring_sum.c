#include <mpi.h> // MPI
#include <stdio.h>

int main(int argc, char *argv[]) {

  // Initialize MPI, get size and rank
  int size, rank, tag = 100;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // IMPLEMENT: Ring sum algorithm
  int sum = rank; // initialize sum
  int rec_rank = rank;
  for (int i = 0; i < 2 * size - 1; i++) {
    if (i % 2 == 0) {
      MPI_Send(&rec_rank, 1, MPI_INTEGER, (rank + 1) % size, tag,
               MPI_COMM_WORLD);

    } else {
      MPI_Recv(&rec_rank, 1, MPI_INTEGER, (rank - 1 + size) % size, tag,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sum += rec_rank;
    }
  }
  printf("Final Process %i: Sum = %i\n", rank, sum);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
