/******************************************************************************
 * FILE: omp_bug1.c
 * DESCRIPTION:
 *   This example attempts to show use of the parallel for construct.  However
 *   it will generate errors at compile time.  Try to determine what is causing
 *   the error.
 ******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 50
#define CHUNKSIZE 5

int main(int argc, char *argv[]) {
  int i, chunk, tid;
  float a[N], b[N], c[N];

  /* Some initializations */
  for (i = 0; i < N; i++)
    a[i] = b[i] = i * 1.0;
  chunk = CHUNKSIZE;

	// This statement is wrong wee need to separate into a parallel section without the  for and schedule part.
  // #pragma omp parallel for shared(a, b, c, chunk) private(i, tid) \
              schedule(static, chunk)
#pragma omp parallel shared(a,b,c,chunk) private(i, tid)
  {
    tid = omp_get_thread_num();
#pragma omp for schedule(static, chunk) 
    for (i = 0; i < N; i++) {
      c[i] = a[i] + b[i];
      printf("tid= %d i= %d c[i]= %f\n", tid, i, c[i]);
    }
  } /* end of parallel for construct */
}
