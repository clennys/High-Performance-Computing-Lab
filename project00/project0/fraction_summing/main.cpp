#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fraction_toolbox.hpp"

using namespace std;

// read command line arguments
static void readcmdline(fraction &frac, int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: n d\n");
    printf("  n        numerator of fraction\n");
    printf("  d        denominator of fraction\n");
    exit(1);
  }

  // read n
  frac.num = atoi(argv[1]);

  // read d
  frac.denom = atoi(argv[2]);
}

static void print_pre_info_test(const fraction &frac) {
  std::string sep = "-------------------------------------------------------\n";
  cout << sep << "Current Fraction for next TEST: ";
  print_fraction(frac);
}

static void test23467(int argc, char *argv[]) {
  // DONE: implement function
  fraction frac;
  readcmdline(frac, argc, argv);

  // Exercise 2
  print_pre_info_test(frac);
  fraction test2 = square_fraction(frac);
  cout << "TEST FUNC 2: Fraction returned by square_fraction: ";
  print_fraction(test2);

  // Exercise 3
  print_pre_info_test(frac);
  square_fraction_inplace(frac);
  cout << "TEST FUN 3: Fraction returned by square_fraction_inplace: ";
  print_fraction(frac);

  // Exercise 4
  print_pre_info_test(frac);
  double frac_value = fraction2double(frac);
  cout << "TEST FUNC 4: Double Value returned by fraction2double: "
       << frac_value << endl;

  // Exercise 6
  print_pre_info_test(frac);
  int great_com_div = gcd(frac);
  cout << "TEST FUNC 6: GCD returned by gcd(fraction frac): " << great_com_div
       << endl;

  // Exercise 7
  print_pre_info_test(frac);
  reduce_fraction_inplace(frac);
  cout << "TEST FUNC 7: Fraction returned by reduce_fraction_inplace: ";
  print_fraction(frac);
}

static void test5() {
  // DONE: implement function
  int a, b;
  cout << "Welcome to the gcd calculator." << endl;
  cout << "Enter first number: ";
  cin >> a;
  cout << "Enter second number: ";
  cin >> b;
  cout << "The gcd of " << a << " and " << b << " is: " << gcd(a, b) << endl;
}

static void test_array_functions(int n) {
  // DONE: implement function
  fraction *frac_array = (fraction *)malloc(n * sizeof(fraction));

  fill_fraction_array(frac_array, n);
  cout << "Filled fraction array with n= " << n << endl;
  // print_fraction_array(frac_array, n);

  cout << "Sum of fractions with sum_fraction_array_approx: "
       << sum_fraction_array_approx(frac_array, n) << endl;

  fraction sum = sum_fraction_array(frac_array, n);
  cout << "Sum of fractions with sum_fraction_array: ";
  print_fraction(sum);
  cout << "Sum of fractions with sum_fraction_array converted to a double: "
       << fraction2double(sum);

  // DONE: find n for which sum function breaks. Explain what is happening.
  // ANSWER: The Function breaks when the common denominator multiplication in
  // the add_fraction function exceeds maximum (2^31) value that an integer can
  // represent. In my implementation this occurs when n > 1292.
	// By taking the decimal value in every addition, we can omit the previously mentioned
  // multiplication and therefore prevent an overflow.
  free(frac_array);
}

static void test_toolbox(int argc, char *argv[]) {
  cout << "\n===============  test23467  =============== " << endl;
  test23467(argc, argv);

  cout << "\n=================  test5  ================= " << endl;
  test5();

  cout << "\n==========  test_array_functions  ========= " << endl;
  int n = 5;
  test_array_functions(n);
}

int main(int argc, char *argv[]) {
  // cout << "\n\n========== Last n before overflow  ========= " << endl;
  // int n = 1289;
  // test_array_functions(n);
  // cout << "\n==========  First n with overflow  ========= " << endl;
  // n++;
  // test_array_functions(n);

  test_toolbox(argc, argv);

  return 0;
}
