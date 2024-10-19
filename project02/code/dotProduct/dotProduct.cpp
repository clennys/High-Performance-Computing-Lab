#include "walltime.h"
#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_ITERATIONS 100

// Example benchmarks
// 0.008s ~0.8MB
// #define N 100000
// 0.1s ~8MB
// #define N 1000000
// 1.1s ~80MB
// #define N 10000000
// 13s ~800MB
// #define N 100000000
// 127s 16GB
// #define N 1000000000
#define EPSILON 0.1

using namespace std;

int main(int argc, char *argv[]) {
  double time_serial, time_start = 0.0;
  double *a, *b;
  int N;

  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <power of 10 for N>" << endl;
    cout << "Example: " << argv[0] << " 6 => N = 10^6" << endl;
    return 1;
  }
  int exp = std::atoi(argv[1]);

  N = static_cast<int>(std::pow(10, exp));

  // Allocate memory for the vectors as 1-D arrays
  a = new double[N];
  b = new double[N];

  // Initialize the vectors with some values
  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = i / 10.0;
  }

  long double alpha = 0;
  // serial execution
  // Note that we do extra iterations to reduce relative timing overhead
  int max_threads = omp_get_max_threads();
  // NOTE: Only compute the serial implementation when max_threads = 1 so we
  // dont have to compute the serial implementation for every number of
  // processes.
  if (max_threads == 1) {
    time_start = wall_time();
    for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
      alpha = 0.0;
      for (int i = 0; i < N; i++) {
        alpha += a[i] * b[i];
      }
    }
    cout << alpha << endl;
    time_serial = wall_time() - time_start;
    cout << "Serial execution time = " << time_serial << " sec" << endl
         << "Serial dot product = " << alpha << endl;
  }

  long double alpha_parallel_red, alpha_parallel_crit, alpha_local = 0;
  double time_red = 0;
  double time_critical = 0;

  //   TODO: Write parallel version (2 ways!)
  //   i.  Using reduction pragma
  //   ii. Using  critical pragma

  // Reduction Implementation
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha_parallel_red = 0.0;
#pragma omp parallel for reduction(+ : alpha_parallel_red)
    for (int i = 0; i < N; i++) {
      alpha_parallel_red += a[i] * b[i];
    }
  }
  time_red = wall_time() - time_start;

  // Critical Implementation
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha_parallel_crit = 0.0;
    alpha_local = 0.0;
#pragma omp parallel firstprivate(alpha_local)
    {
#pragma omp for
      for (int i = 0; i < N; i++) {
        alpha_local += a[i] * b[i];
      }
#pragma omp critical
      {
        alpha_parallel_crit += alpha_local;
      }
    }
  }

  time_critical = wall_time() - time_start;

  // NOTE: Commented this part out, so I don't have to redo the serial execution
  // for every number of max threads
  //
  // if ((fabs(alpha_parallel_red - alpha) / fabs(alpha_parallel_red)) >
  // EPSILON) {
  //   cout << "parallel reduction: " << alpha_parallel_red
  //        << ", serial: " << alpha << "\n";
  //   cerr << "Alpha not yet implemented correctly!\n";
  //   exit(1);
  // }
  // if ((fabs(alpha_parallel_crit - alpha) / fabs(alpha_parallel_crit)) >
  //     EPSILON) {
  //   cout << "parallel critical: " << alpha_parallel_crit
  //        << ", serial: " << alpha << "\n";
  //   cerr << "Alpha not yet implemented correctly!\n";
  //   exit(1);
  // }
  cout << "Parallel dot product reduction = " << alpha_parallel_red << endl
       << "Parallel dot product critical = " << alpha_parallel_crit << endl
       << "Time reduction method = " << time_red << " sec" << endl
       << "Time critical method = " << time_critical << " sec" << endl;

  // De-allocate memory
  delete[] a;
  delete[] b;

  return 0;
}
