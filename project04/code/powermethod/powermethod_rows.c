#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h> // MPI

#include "walltime.h"

/*******************************************************************************
 *
 * Implement the power method for an n x n matrix A, stored in row-major order,
 * and rows distributed across "size" MPI processes. Your task is to complete
 * the "To do" items, and perform a strong and weak scaling study.
 *
 * To test your implementation, test cases 1, 2 and 3 have known largest
 *absolute eigenvalue.
 *
 *******************************************************************************/

int main(int argc, char *argv[]) {

  // Seed random number generator
  srand(42);

  // Initialize MPI, get size and rank
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Process command line arguments
  if (argc < 5) {
    if (rank == 0) {
      printf("Usage: powermethod_rows test_case n niter tol\n");
      printf("       test_case: test case to run");
      printf("       n        : matrix size");
      printf("       niter    : maximum number of iterations");
      printf("       tol      : tolerance to reach");
    }
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }
  int test_case = atoi(argv[1]);
  int n = atoi(argv[2]);
  int niter = atoi(argv[3]);
  double tol = atof(argv[4]);
  if (n < size) {
    if (rank == 0)
      printf("Error: number of rows cannot be smaller than "
             "number of processes!\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }

  // Give some info
  if (rank == 0) {
    printf("Doing %d power iterations on %dx%d matrix distributed row-wise"
           " over %d processes...\n",
           niter, n, n, size);
  }

  // Partition work evenly among processes
  int nrows_local, row_beg_local, row_end_local;
  // printf("[Proc %3d] Doing rows %d to %d\n", rank, row_beg_local,
  // row_end_local);
  // TODO: Partition the "n" rows of the matrix evenly among the "size" MPI
  //        processes.
  // Hint : The first "n % size" processes get "n / size + 1" rows, while the
  //        remaining processes get "n / size".
  nrows_local = n / size + (rank < n % size ? 1 : 0);
  row_beg_local = rank * (n / size) + (rank < n % size ? rank : n % size);
  row_end_local = row_beg_local + nrows_local;
  // printf("[Proc %3d] Doing rows %d to %d\n", rank, row_beg_local,
         // row_end_local);
  // Initialize matrix A
  double *A = (double *)calloc(nrows_local * n, sizeof(double));
  for (int i_local = 0; i_local < nrows_local; ++i_local) {
    int i_global = row_beg_local + i_local;
    for (int j_global = 0; j_global < n; ++j_global) {
      switch (test_case) {
      case 1:
        // Test case 1: A(i, j) = 1 for 1 <= i, j <= n
        //              theta = n
        A[i_local * n + j_global] = 1.;
        break;
      case 2:
        // Test case 2: A(i, j) = 1 if i == j for 1 <= i, j <= n
        //              theta = 1
        if (i_global == j_global) {
          A[i_local * n + j_global] = 1.;
        } else {
          A[i_local * n + j_global] = 0.;
        }
        break;
      case 3:
        // Test case 3: A(i, j) = i if i == j for 1 <= i, j <= n
        //              theta = n
        if (i_global == j_global) {
          A[i_local * n + j_global] = i_global + 1;
        } else {
          A[i_local * n + j_global] = 0.;
        }
        break;
      case 4:
        // Test case 4: A(i, j) = uniform random number [0, 1[ for 1 <= i, j <=
        // n
        //              theta = unknown
        A[i_local * n + j_global] = (double)rand() / (double)(RAND_MAX + 1u);
        break;
      default:
        if (rank == 0)
          printf("Error: test_case = 1, 2, 3 or 4!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
      }
    }
  }
  // Initialize vector y
  double *y = (double *)calloc(n, sizeof(double));
  if (rank == 0) {
    // Random vector
    for (int i_global = 0; i_global < n; ++i_global) {
      y[i_global] = rand();
    }
    // Norm of vector y
    double norm2 = 0.;
    for (int i_global = 0; i_global < n; ++i_global) {
      norm2 += y[i_global] * y[i_global];
    }
    double norm = sqrt(norm2);
    // Normalize vector y
    for (int i_global = 0; i_global < n; ++i_global) {
      y[i_global] /= norm;
    }
  }
  // TODO: Broadcast the random initial guess vector to all MPI processes.
  // Hint : MPI_Bcast.
  MPI_Bcast(y, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  int *recvcounts = (int *)calloc(size, sizeof(int));
  int *displs = (int *)calloc(size, sizeof(int));

  for (int i = 0; i < size; ++i) {
    recvcounts[i] = n / size + (i < n % size ? 1 : 0);
    displs[i] = (i == 0) ? 0 : displs[i - 1] + recvcounts[i - 1];
  }

  // Power method
  double theta, theta_local, error;
  double *y_local = (double *)calloc(nrows_local, sizeof(double));
  double *v = (double *)calloc(n, sizeof(double));
  double *v_local = (double *)calloc(nrows_local, sizeof(double));
  int iter;
  double time_start = walltime();
  for (iter = 0; iter < niter; ++iter) {
    // TODO: Implement parallel power method here.
    // HINT: Do the matrix-vector multiply y = A v below over the local
    //        (to the process) rows, and use MPI_Allgather / MPI_Allgatherv
    //        to synchronize the result.
    // Normalize vector: v = y / || y ||_2
    double norm2, norm2_local = 0.;
    for (int i_global = row_beg_local; i_global < row_end_local; ++i_global) {
      norm2_local += y[i_global] * y[i_global];
    }

    MPI_Allreduce(&norm2_local, &norm2, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    double norm = sqrt(norm2);

    for (int i_local = 0; i_local < nrows_local; ++i_local) {
      v_local[i_local] = y[row_beg_local + i_local] / norm;
    }
    MPI_Allgatherv(v_local, nrows_local, MPI_DOUBLE, v, recvcounts, displs,
                   MPI_DOUBLE, MPI_COMM_WORLD);
    // Matrix-vector multiply: y = A v
    // Hint: Compute only the local rows, save them in the buffer y_local
    //       and synchronize the result using MPI_Allgather / MPI_Allgatherv.
    for (int i_local = 0; i_local < nrows_local; ++i_local) {
      y_local[i_local] = 0.;
      for (int j_global = 0; j_global < n; ++j_global) {
        y_local[i_local] += A[i_local * n + j_global] * v[j_global];
      }
    }
    MPI_Allgatherv(y_local, nrows_local, MPI_DOUBLE, y, recvcounts, displs,
                   MPI_DOUBLE, MPI_COMM_WORLD);
    // Compute eigenvalue: theta = v^T y
    theta_local = 0.;
    for (int i_global = row_beg_local; i_global < row_end_local; ++i_global) {
      theta_local += v[i_global] * y[i_global];
    }
    MPI_Allreduce(&theta_local, &theta, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Check convergence
    double error2, error2_local = 0.;
    for (int i_global = row_beg_local; i_global < row_end_local; ++i_global) {
      error2_local += (y[i_global] - theta * v[i_global]) *
                      (y[i_global] - theta * v[i_global]);
    }
    MPI_Allreduce(&error2_local, &error2, 1, MPI_DOUBLE, MPI_SUM,
                  MPI_COMM_WORLD);
    error = sqrt(error2);
    // if (rank == 0)
    //   printf("iteration / theta/ error: %4d / %15.5f / %25.15e\n", iter, theta,
    //          error);
    if (error < tol * fabs(theta))
      break;
  }
  double time_end = walltime();

  // Report result
  if (rank == 0) {
    if (iter < niter) {
      printf("Power method: Converged in ");
    } else {
      printf("Power method: NOT converged in ");
    }
    printf("%d iteration(s) "
           "to largest absolute eigenvalue theta = %f "
           "in %f second(s) time.\n",
           iter, theta, (time_end - time_start));
    printf("### %d, %d, %d, %f, %f ###\n", size, n, iter, theta,
           (time_end - time_start));
  }

  // Free
  free(A);
  free(y);
  free(y_local);
  free(v_local);
  free(v);
  free(recvcounts);
  free(displs);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
