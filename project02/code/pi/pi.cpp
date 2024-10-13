#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  long long int N = 10000000000; // <<< carefull! This larger than regular
  // int (its a long int)
  // long int N =
  //     10000; // <<< carefull! This larger than regular int (its a long int)
  double dx = 1. / double(N);
  double sum, x = 0;
  double pi = 0;
  double time = 0;

  /* TODO: Serial Version of Computing Pi*/
  time = omp_get_wtime();
  // TODO: Fill this gap
  for (long long int i = 0; i < N; i++) {
    x = dx * (i + 0.5);
    sum += 4 / (1 + x * x);
  }
  pi = sum * dx;
  time = omp_get_wtime() - time;
  printf("pi=%e, N=%ld, time_srl=%e secs\n", pi, N, time);

  /* TODO: Parallel Version of Computing Pi*/
  time = omp_get_wtime();
  sum = 0.;
  // TODO: Fill this gap
#pragma omp parallel for reduction(+ : sum)
  for (long long int i = 0; i < N; i++) {
    x = dx * (i + 0.5);
    sum += 4 / (1 + x * x);
  }
  pi = sum * dx;
  time = omp_get_wtime() - time;
  printf("pi=%e, N=%ld, time_plr=%e secs\n", pi, N, time);

  return 0;
}
