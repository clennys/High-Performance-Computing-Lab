/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS-USI Summer School     *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS/USI take no responsibility for the use of the enclosed  *
 * teaching material.                                           *
 *                                                              *
 * Purpose: Exchange ghost cell in 2 directions using a topology*
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

/* Use only 16 processes for this exercise
 * Send the ghost cell in two directions: left<->right and top<->bottom
 * ranks are connected in a cyclic manner, for instance, rank 0 and 12 are
 * connected
 *
 * process decomposition on 4*4 grid
 *
 * |-----------|
 * | 0| 1| 2| 3|
 * |-----------|
 * | 4| 5| 6| 7|
 * |-----------|
 * | 8| 9|10|11|
 * |-----------|
 * |12|13|14|15|
 * |-----------|
 *
 * Each process works on a 6*6 (SUBDOMAIN) block of data
 * the D corresponds to data, g corresponds to "ghost cells"
 * xggggggggggx
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * xggggggggggx
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SUBDOMAIN 6
#define DOMAINSIZE (SUBDOMAIN + 2)

int main(int argc, char *argv[]) {
  int rank, size, i, j, dims[2], periods[2], rank_top, rank_bottom, rank_left,
      rank_right, coords[2];
  double data[DOMAINSIZE * DOMAINSIZE];
  MPI_Request request;
  MPI_Status status;
  MPI_Comm comm_cart;
  MPI_Datatype data_ghost;

  // Initialize MPI
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 16) {
    printf("please run this with 16 processors\n");
    MPI_Finalize();
    exit(1);
  }

  // initialize the domain
  for (i = 0; i < DOMAINSIZE * DOMAINSIZE; i++) {
    data[i] = rank;
  }

  // TODO: set the dimensions of the processor grid and periodic boundaries in
  // both dimensions
  dims[0] = 4;
  dims[1] = 4;
  periods[0] = 1;
  periods[1] = 1;

  // TODO: Create a Cartesian communicator (4*4) with periodic boundaries (we do
  // not allow the reordering of ranks) and use it to find your neighboring
  // ranks in all dimensions in a cyclic manner.
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_cart);
  MPI_Cart_coords(comm_cart, rank, 2, coords);
  printf("Rank %d coords (%d, %d)\n", rank, coords[0], coords[1]);

  // TODO: find your top/bottom/left/right neighbor using the new communicator,
  // see MPI_Cart_shift() rank_top, rank_bottom rank_left, rank_right
  MPI_Cart_shift(comm_cart, 0, 1, &rank_top, &rank_bottom);
  MPI_Cart_shift(comm_cart, 1, 1, &rank_left, &rank_right);
  printf("Rank %d with coords (%d, %d) and neighbors: top=%d, bottom=%d, "
         "left=%d, right=%d\n",
         rank, coords[0], coords[1], rank_top, rank_bottom, rank_left,
         rank_right);

  // Ordinal Directions
  int top_right_rank, top_left_rank, bottom_right_rank, bottom_left_rank;
  int top_right_coords[2] = {coords[0] - 1, coords[1] + 1};
  MPI_Cart_rank(comm_cart, top_right_coords, &top_right_rank);
  int top_left_coords[2] = {coords[0] - 1, coords[1] - 1};
  MPI_Cart_rank(comm_cart, top_left_coords, &top_left_rank);
  int bottom_right_coords[2] = {coords[0] + 1, coords[1] + 1};
  MPI_Cart_rank(comm_cart, bottom_right_coords, &bottom_right_rank);
  int bottom_left_coords[2] = {coords[0] + 1, coords[1] - 1};
  MPI_Cart_rank(comm_cart, bottom_left_coords, &bottom_left_rank);

  //  TODO: create derived datatype data_ghost, create a datatype for sending
  //  the column, see MPI_Type_vector() and MPI_Type_commit()
  // data_ghost
  MPI_Datatype T_row, T_col;
  MPI_Type_contiguous(SUBDOMAIN, MPI_DOUBLE, &T_row);
  MPI_Type_vector(SUBDOMAIN, 1, DOMAINSIZE, MPI_DOUBLE, &T_col);
  MPI_Type_commit(&T_col);
  MPI_Type_commit(&T_row);

  //  TODO: ghost cell exchange with the neighbouring cells in all directions
  //  use MPI_Irecv(), MPI_Send(), MPI_Wait() or other viable alternatives

  //  to the top
  MPI_Sendrecv(&data[1 * DOMAINSIZE + 1], 1, T_row, rank_top, 0,
               &data[(SUBDOMAIN + 1) * DOMAINSIZE + 1], 1, T_row, rank_bottom,
               0, comm_cart, MPI_STATUS_IGNORE);

  //  to the bottom
  MPI_Sendrecv(&data[SUBDOMAIN * DOMAINSIZE + 1], 1, T_row, rank_bottom, 0,
               &data[0 * DOMAINSIZE + 1], 1, T_row, rank_top, 0, comm_cart,
               MPI_STATUS_IGNORE);

  //  to the left
  MPI_Sendrecv(&data[1 * DOMAINSIZE + 1], 1, T_col, rank_left, 0,
               &data[1 * DOMAINSIZE + (SUBDOMAIN + 1)], 1, T_col, rank_right, 0,
               comm_cart, MPI_STATUS_IGNORE);

  //  to the right
  MPI_Sendrecv(&data[1 * DOMAINSIZE + SUBDOMAIN], 1, T_col, rank_right, 0,
               &data[1 * DOMAINSIZE + 0], 1, T_col, rank_left, 0, comm_cart,
               MPI_STATUS_IGNORE);

  // to top right
  MPI_Sendrecv(&data[1 * DOMAINSIZE + (DOMAINSIZE - 2)], 1, MPI_DOUBLE,
               top_right_rank, 0, &data[(DOMAINSIZE - 1) * DOMAINSIZE + 0], 1,
               MPI_DOUBLE, bottom_left_rank, 0, comm_cart, MPI_STATUS_IGNORE);

  // to top left
  MPI_Sendrecv(&data[1 * DOMAINSIZE + 1], 1, MPI_DOUBLE, top_left_rank, 0,
               &data[(DOMAINSIZE - 1) * DOMAINSIZE + (DOMAINSIZE - 1)], 1,
               MPI_DOUBLE, bottom_right_rank, 0, comm_cart, MPI_STATUS_IGNORE);

  // to bottom right
  MPI_Sendrecv(&data[(DOMAINSIZE - 2) * DOMAINSIZE + (DOMAINSIZE - 2)], 1,
               MPI_DOUBLE, bottom_right_rank, 0, &data[0 * DOMAINSIZE + 0], 1,
               MPI_DOUBLE, top_left_rank, 0, comm_cart, MPI_STATUS_IGNORE);
  // to bottom left
  MPI_Sendrecv(&data[(DOMAINSIZE - 2) * DOMAINSIZE + 1], 1, MPI_DOUBLE,
               bottom_left_rank, 0, &data[0 * DOMAINSIZE + (DOMAINSIZE - 1)], 1,
               MPI_DOUBLE, top_right_rank, 0, comm_cart, MPI_STATUS_IGNORE);

  MPI_Barrier(comm_cart);
  if (rank == 9) {
    printf("data of rank 9 after communication\n");
    for (j = 0; j < DOMAINSIZE; j++) {
      for (i = 0; i < DOMAINSIZE; i++) {
        printf("%4.1f ", data[i + j * DOMAINSIZE]);
      }
      printf("\n");
    }
  }

  // Free MPI resources (e.g., types and communicators)
  // TODO
  MPI_Type_free(&T_col);
  MPI_Type_free(&T_row);
  MPI_Comm_free(&comm_cart);

  MPI_Finalize();

  return 0;
}
