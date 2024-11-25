//******************************************
// operators.cpp
// based on min-app code written by Oliver Fuhrer, MeteoSwiss
// modified by Ben Cumming, CSCS
// *****************************************

// Description: Contains simple operators which can be used on 2/3d-meshes

#include "operators.h"
#include "data.h"
#include "stats.h"
#include <iostream>
#include <mpi.h>

namespace operators {

// compute the diffusion-reaction stencils
// s_old is the population concentration at time step k-1, s_new at k,
// and f is the residual (see Eq. (7) in Project 3).
void diffusion(data::Field const &s_old, data::Field const &s_new,
               data::Field &f) {
  using data::domain;
  using data::options;

  using data::bndE;
  using data::bndN;
  using data::bndS;
  using data::bndW;

  using data::buffE;
  using data::buffN;
  using data::buffS;
  using data::buffW;

  double alpha = options.alpha;
  double beta = options.beta;

  int nx = domain.nx;
  int ny = domain.ny;
  int iend = nx - 1;
  int jend = ny - 1;

  // TODO: exchange the ghost cells using non-blocking point-to-point
  //       communication
  MPI_Request send_req[4];
  MPI_Request recv_req[4];

  for (int j = 0; j < ny; j++) {
    buffE[j] = s_new(iend, j);
    buffW[j] = s_new(0, j);
  }

  for (int i = 0; i < nx; i++) {
    buffN[i] = s_new(i, jend);
    buffS[i] = s_new(i, 0);
  }

  // Send/recv east
  MPI_Isend(buffE.data(), buffE.xdim(), MPI_DOUBLE, domain.neighbour_east, 0,
            domain.comm_cart, &send_req[0]);
  MPI_Irecv(bndE.data(), bndE.xdim(), MPI_DOUBLE, domain.neighbour_east, 1,
            domain.comm_cart, &recv_req[1]);
  // Send/recv west
  MPI_Isend(buffW.data(), buffW.xdim(), MPI_DOUBLE, domain.neighbour_west, 1,
            domain.comm_cart, &send_req[1]);
  MPI_Irecv(bndW.data(), bndW.xdim(), MPI_DOUBLE, domain.neighbour_west, 0,
            domain.comm_cart, &recv_req[0]);

  // Send/recv north
  MPI_Isend(buffN.data(), buffN.xdim(), MPI_DOUBLE, domain.neighbour_north, 2,
            domain.comm_cart, &send_req[2]);
  MPI_Irecv(bndN.data(), bndN.xdim(), MPI_DOUBLE, domain.neighbour_north, 3,
            domain.comm_cart, &recv_req[3]);

  // Send/recv south
  MPI_Isend(buffS.data(), buffS.xdim(), MPI_DOUBLE, domain.neighbour_south, 3,
            domain.comm_cart, &send_req[3]);
  MPI_Irecv(bndS.data(), bndS.xdim(), MPI_DOUBLE, domain.neighbour_south, 2,
            domain.comm_cart, &recv_req[2]);

  // the interior grid points
  for (int j = 1; j < jend; j++) {
    for (int i = 1; i < iend; i++) {
      f(i, j) = -(4. + alpha) * s_new(i, j)         // central point
                + s_new(i - 1, j) + s_new(i + 1, j) // east and west
                + s_new(i, j - 1) + s_new(i, j + 1) // north and south
                + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }
  }
  MPI_Waitall(4, recv_req, MPI_STATUSES_IGNORE);
  // east boundary
  {
    int i = nx - 1;
    for (int j = 1; j < jend; j++) {
      f(i, j) = -(4. + alpha) * s_new(i, j) + s_new(i - 1, j) + bndE[j] +
                s_new(i, j - 1) + s_new(i, j + 1) + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }
  }

  // west boundary
  {
    int i = 0;

    for (int j = 1; j < jend; j++) {
      f(i, j) = -(4. + alpha) * s_new(i, j) + bndW[j] + s_new(i + 1, j) +
                s_new(i, j - 1) + s_new(i, j + 1) + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }
  }

  // north boundary (plus NE and NW corners)
  {
    int j = ny - 1;

    {
      int i = 0; // NW corner
      f(i, j) = -(4. + alpha) * s_new(i, j) + bndW[j] + s_new(i + 1, j) +
                s_new(i, j - 1) + bndN[i] + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    // north boundary
    for (int i = 1; i < iend; i++) {
      f(i, j) = -(4. + alpha) * s_new(i, j) + s_new(i - 1, j) +
                s_new(i + 1, j) + s_new(i, j - 1) + bndN[i] +
                alpha * s_old(i, j) + beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    {
      int i = nx - 1; // NE corner
      f(i, j) = -(4. + alpha) * s_new(i, j) + s_new(i - 1, j) + bndE[j] +
                s_new(i, j - 1) + bndN[i] + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }
  }

  // south boundary (plus SW and SE corners)
  {
    int j = 0;
    {
      int i = 0; // SW corner
      f(i, j) = -(4. + alpha) * s_new(i, j) + bndW[j] + s_new(i + 1, j) +
                bndS[i] + s_new(i, j + 1) + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    // south boundary
    for (int i = 1; i < iend; i++) {
      f(i, j) = -(4. + alpha) * s_new(i, j) + s_new(i - 1, j) +
                s_new(i + 1, j) + bndS[i] + s_new(i, j + 1) +
                alpha * s_old(i, j) + beta * s_new(i, j) * (1.0 - s_new(i, j));
    }

    {
      int i = nx - 1; // SE corner
      f(i, j) = -(4. + alpha) * s_new(i, j) + s_new(i - 1, j) + bndE[j] +
                bndS[i] + s_new(i, j + 1) + alpha * s_old(i, j) +
                beta * s_new(i, j) * (1.0 - s_new(i, j));
    }
  }

  MPI_Waitall(4, send_req, MPI_STATUSES_IGNORE);

  // Accumulate the flop counts
  // 8 ops total per point
  stats::flops_diff += 12 * (nx - 2) * (ny - 2) // interior points
                       + 11 * (nx - 2 + ny - 2) // NESW boundary points
                       + 11 * 4;                // corner points
}

} // namespace operators
