#ifndef CONSTS_H
#define CONSTS_H
#include <mpi.h>

// maximum number of iterations
#define MAX_ITERS 10000

// image size
#define IMAGE_WIDTH 4096
#define IMAGE_HEIGHT 4096

// the extent of the parameter plane ( MIN_X + iMIN_Y <= c < MAX_X + iMAX_Y )
#define MIN_X -2.1
#define MAX_X 0.7
#define MIN_Y -1.4
#define MAX_Y 1.4

typedef struct {
  long nx;     // Local  domain size in x-direction
  long ny;     // Local  domain size in y-direction
  long startx; // Global domain start index of local domain in x-direction
  long starty; // Global domain start index of local domain in y-direction
  long endx;   // Global domain end   index of local domain in x-direction
  long endy;   // Global domain end   index of local domain in y-direction
} Domain;

typedef struct {
  int x;         // x-coord. of current MPI process within the process grid
  int y;         // y-coord. of current MPI process within the process grid
  int nx;        // Number of processes in x-direction
  int ny;        // Number of processes in y-direction
  MPI_Comm comm; // (Cartesian) MPI communicator
} Partition;

/**
Create Partition structure that represents the layout of the grid of processes
organized in the Cartesian communicator (p.comm) that needs to be created
(MPI_Cart_create) and contains information such as number of processes in x and
y direction (p.nx, p.ny) and the coordinates of the current MPI process
(p.x, p.y).
*/
Partition createPartition(int mpi_rank, int mpi_size) {
  Partition p;

  // TODO: determine size of the grid of MPI processes (p.nx, p.ny), see
  // MPI_Dims_create()
  int dims[2] = {0, 0}; // MPI_Dims_create decides the dimensions
  MPI_Dims_create(mpi_size, 2, dims);
  p.nx = dims[0];
  p.ny = dims[1];

  // TODO: Create cartesian communicator (p.comm), we do not allow the
  // reordering of ranks here, see MPI_Cart_create()
  int periods[2] = {0, 0}; // Non-periodic
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &p.comm);

  // TODO: Determine the coordinates in the Cartesian grid (p.x, p.y), see
  // MPI_Cart_coords()
  int coords[2];
  MPI_Cart_coords(p.comm, mpi_rank, 2, coords);
  p.x = coords[0];
  p.y = coords[1];

  return p;
}

/**
Updates Partition structure to represent the process mpi_rank.
Copy the grid information (p.nx, p.ny and p.comm) and update
the coordinates to represent position in the grid of the given
process (mpi_rank)
*/
Partition updatePartition(Partition p_old, int mpi_rank) {
  Partition p;

  // copy grid dimension and the communicator
  p.ny = p_old.ny;
  p.nx = p_old.nx;
  p.comm = p_old.comm;

  // TODO: update the coordinates in the cartesian grid (p.x, p.y) for given
  // mpi_rank, see MPI_Cart_coords()
  int coords[2];
  MPI_Cart_coords(p.comm, mpi_rank, 2, coords);
  p.x = coords[0];
  p.y = coords[1];

  return p;
}

/**
Create Domain structure that represents the information about the local domain
of the current MPI process. It holds information such as the size of the local
domain (number of pixels in each dimension - d.nx, d.ny) and its global indices
(index of the first and the last pixel in the full image of the Mandelbrot set
that will be computed by the current process d.startx, d.endx and d.starty,
d.endy).
*/
Domain createDomain(Partition p) {
  Domain d;

  // TODO: compute size of the local domain
  d.nx = IMAGE_WIDTH / p.nx;
  d.ny = IMAGE_HEIGHT / p.ny;

  // TODO: compute index of the first pixel in the local domain
  d.startx = p.x * d.nx;
  d.starty = p.y * d.ny;

  // TODO: compute index of the last pixel in the local domain
  d.endx = d.startx + d.nx - 1;
  d.endy = d.startx + d.ny - 1;

  return d;
}

#endif /* CONSTS_H */
