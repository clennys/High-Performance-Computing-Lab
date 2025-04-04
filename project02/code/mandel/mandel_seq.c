#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "pngwriter.h"
#include "walltime.h"
#include "omp.h"

// #define SERIAL

int main(int argc, char **argv) {
  png_data *pPng = png_create(IMAGE_WIDTH, IMAGE_HEIGHT);

  double x, y, x2, y2, cx, cy;

  double fDeltaX = (MAX_X - MIN_X) / (double)IMAGE_WIDTH;
  double fDeltaY = (MAX_Y - MIN_Y) / (double)IMAGE_HEIGHT;

  long nTotalIterationsCount = 0;

  long i, j;

  int max_threads = omp_get_max_threads();
  double time_start = walltime();
  // do the calculation
  if (max_threads == 1) {
    cy = MIN_Y;
    for (j = 0; j < IMAGE_HEIGHT; j++) {
      cx = MIN_X;
      for (i = 0; i < IMAGE_WIDTH; i++) {
        x = cx;
        y = cy;
        x2 = x * x;
        y2 = y * y;
        // compute the orbit z, f(z), f^2(z), f^3(z), ...
        // count the iterations until the orbit leaves the circle |z|=2.
        // stop if the number of iterations exceeds the bound MAX_ITERS.
        int n = 0;
        // TODO:
        // >>>>>>>> CODE IS MISSING

        // |c| = sqrt(x^2 + y^2) < 2
        // Square |c| < 2 -> no sqrt need
        while (x2 + y2 < 4 && n < MAX_ITERS) {
          // z1*z2 = (x1x2 - y1y2) + i(x1y2 + x2y1)
          // z^2 = (xx - yy) + i(xy + xy)
          // z^2 + c = (xx - yy + cx) + i(xy + xy + cy)
          // x = x^2 - y^2 + cx
          // y = 2xy + cy
          y = 2 * x * y + cy;
          x = x2 - y2 + cx;
          x2 = x * x;
          y2 = y * y;
          n++;
          nTotalIterationsCount++;
        }

        // <<<<<<<< CODE IS MISSING
        // n indicates if the point belongs to the mandelbrot set
        // plot the number of iterations at point (i, j)
        int c = ((long)n * 255) / MAX_ITERS;
        // Only write to plot once for (i,j) no race condition here.
        png_plot(pPng, i, j, c, c, c);
        // TODO: ?
        cx += fDeltaX;
      }
      // TODO: ?
      // i0 : cy = MIN_Y + fdeltaY = cy + 1*fdeltaY
      // i1 : cy = MIN_Y + fdeltaY + fdeltaY = cy + 2 * fdeltaY
      // ij : cy = MIN_Y + (j+1) * fdeltaY
      cy += fDeltaY;
    }
  } else {
#pragma omp parallel private(i, j, cx, cy, x, y, x2, y2)                       \
    shared(pPng, fDeltaX, fDeltaY) reduction(+ : nTotalIterationsCount)
    {
#pragma omp for collapse(2) schedule(static)
      for (j = 0; j < IMAGE_HEIGHT; j++) {
        for (i = 0; i < IMAGE_WIDTH; i++) {
          cy = MIN_Y + j * fDeltaY;
          cx = MIN_X + i * fDeltaY;
          x = cx;
          y = cy;
          x2 = x * x;
          y2 = y * y;
          // compute the orbit z, f(z), f^2(z), f^3(z), ...
          // count the iterations until the orbit leaves the circle |z|=2.
          // stop if the number of iterations exceeds the bound MAX_ITERS.
          int n = 0;
          // TODO:
          // >>>>>>>> CODE IS MISSING

          // |c| = sqrt(x^2 + y^2) < 2
          // Square |c| < 2 -> no sqrt need
          while (x2 + y2 < 4 && n < MAX_ITERS) {
            // z1*z2 = (x1x2 - y1y2) + i(x1y2 + x2y1)
            // z^2 = (xx - yy) + i(xy + xy)
            // z^2 + c = (xx - yy + cx) + i(xy + xy + cy)
            // x = x^2 - y^2 + cx
            // y = 2xy + cy
            y = 2 * x * y + cy;
            x = x2 - y2 + cx;
            x2 = x * x;
            y2 = y * y;
            n++;
          }
          nTotalIterationsCount += n;

          // <<<<<<<< CODE IS MISSING
          // n indicates if the point belongs to the mandelbrot set
          // plot the number of iterations at point (i, j)
          int c = ((long)n * 255) / MAX_ITERS;
          // Only write to plot once for (i,j) no race condition here.
          png_plot(pPng, i, j, c, c, c);
        }
      }
    }
  }
  double time_end = walltime();

  // print benchmark data
  printf("Total time:                 %g seconds\n", (time_end - time_start));
  printf("Image size:                 %ld x %ld = %ld Pixels\n",
         (long)IMAGE_WIDTH, (long)IMAGE_HEIGHT,
         (long)(IMAGE_WIDTH * IMAGE_HEIGHT));
  printf("Total number of iterations: %ld\n", nTotalIterationsCount);
  printf("Avg. time per pixel:        %g seconds\n",
         (time_end - time_start) / (double)(IMAGE_WIDTH * IMAGE_HEIGHT));
  printf("Avg. time per iteration:    %g seconds\n",
         (time_end - time_start) / (double)nTotalIterationsCount);
  printf("Iterations/second:          %g\n",
         nTotalIterationsCount / (time_end - time_start));
  // assume there are 8 floating point operations per iteration
  printf("MFlop/s:                    %g\n",
         nTotalIterationsCount * 8.0 / (time_end - time_start) * 1.e-6);

  png_write(pPng, "mandel.png");
  return 0;
}
