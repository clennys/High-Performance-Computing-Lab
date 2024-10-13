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
  int N, p;

  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <number of threads> <power of 10 for N>"
         << endl;
    cout << "Example: " << argv[0] << " 4 6 => 4 threads and N = 10^6" << endl;
    return 1;
  }
  p = std::atoi(argv[1]);
  int exp = std::atoi(argv[2]);

  if (p <= 0 || p > omp_get_max_threads() || exp < 0 || exp > 9) {
    cerr << "Number of threads p must be p > 0 and p <= p_max" << endl
         << "And power of 10 must be "
            "non-negative and the exponent cannot be higher than 9."
         << endl;
    return 1; // Return an error code
  }

  N = static_cast<int>(std::pow(10, exp));

  omp_set_dynamic(0);
  omp_set_num_threads(p);

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
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha = 0.0;
    for (int i = 0; i < N; i++) {
      alpha += a[i] * b[i];
    }
  }
  time_serial = wall_time() - time_start;
  cout << "Serial execution time = " << time_serial << " sec" << endl;

  long double alpha_parallel = 0;
  double time_red = 0;
  double time_critical = 0;

  //   TODO: Write parallel version (2 ways!)
  //   i.  Using reduction pragma
  //   ii. Using  critical pragma

  // Reduction Implementation
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha_parallel = 0.0;
#pragma omp parallel for reduction(+ : alpha_parallel)
    for (int i = 0; i < N; i++) {
      alpha_parallel += a[i] * b[i];
    }
  }
  time_red = wall_time() - time_start;

  // Critical Implementation
  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++) {
    alpha_parallel = 0.0;
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
#pragma omp critical
      {
        alpha_parallel += a[i] * b[i];
      }
    }
  }

  time_critical = wall_time() - time_start;

  if ((fabs(alpha_parallel - alpha) / fabs(alpha_parallel)) > EPSILON) {
    cout << "parallel reduction: " << alpha_parallel << ", serial: " << alpha
         << "\n";
    cerr << "Alpha not yet implemented correctly!\n";
    exit(1);
  }
  cout << "Parallel dot product = " << alpha_parallel << endl
       << "Time reduction method = " << time_red << " sec" << endl
       << "Time critical method = " << time_critical << " sec" << endl;

  // De-allocate memory
  delete[] a;
  delete[] b;

  return 0;
}
