#include <iostream>

#include "fraction_toolbox.hpp"

void print_fraction(fraction frac) {
  std::cout << frac.num << '/' << frac.denom << std::endl;
}

void print_fraction_array(fraction frac_array[], int n) {
  std::cout << "[ " << frac_array[0].num << '/' << frac_array[0].denom
            << std::endl;
  for (int i = 1; i < n - 1; i++) {
    std::cout << "  ";
    print_fraction(frac_array[i]);
  }
  std::cout << "  " << frac_array[n - 1].num << '/' << frac_array[n - 1].denom
            << " ]" << std::endl;
}

// DONE: implement function 2
fraction square_fraction(fraction frac) {
  frac.num *= frac.num;
  frac.denom *= frac.denom;
  return frac;
}

// DONE: implement function 3
void square_fraction_inplace(fraction &frac) {
  frac.num *= frac.num;
  frac.denom *= frac.denom;
}

double fraction2double(fraction frac) {
  // DONE: implement function 4
  return double(frac.num) / frac.denom;
}

int gcd(int a, int b) {
  // DONE: implement function 5
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}

// DONE: implement function 6
int gcd(fraction frac) {
  int a = frac.num;
  int b = frac.denom;
  while (b != 0) {
    int tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}

void reduce_fraction_inplace(fraction &frac) {
  // DONE: implement function 7
  // DONE: add short comment to explain which of the gcd() functions your code
  // is calling?
  // ANSWER: It's calling the gcd(fraction frac) function which uses the
  // iterative Euclid's algorithm on lines [48 - 57]. The compiler chooses the
  // appropriate function based on the arguments given in the function call.
  // Hence by passing the fraction type in the function call, I know the
  // function on lines [48-57] is called.
  int gcd_num = gcd(frac);
  frac.num /= gcd_num;
  frac.denom /= gcd_num;
}

fraction add_fractions(fraction frac1, fraction frac2) {
  // DONE: implement function 8
  int lcm = frac1.denom * frac2.denom /
            gcd(frac1.denom, frac2.denom); // Denom cannot be 0!
  int factor1 = lcm / frac1.denom;
  int factor2 = lcm / frac2.denom;
  fraction result(factor1 * frac1.num + factor2 * frac2.num, lcm);
  if (result.num < 0 || result.denom < 1) {
    std::cout << "Integer Overflow!!!!" << std::endl;
  }
  reduce_fraction_inplace(result);
  return result;
}

double sum_fraction_array_approx(fraction frac_array[], int n) {
  // DONE: implement function 9
  // DONE: Add short comment to explain why this function is approximate
  // ANSWER: Converting the fraction to a double can result in precision loss.
  // Floating-points (i.e. type double) have limited precision and are unable to
  // precisely represent all real numbers due to limited storage.
  // Floating-points especially struggle with very small/large or repeating
  // values.

  double sum = 0.0;
  for (int i = 0; i < n; i++) {
    sum += fraction2double(frac_array[i]);
  }
  return sum;
}

// DONE: implement function 10
fraction sum_fraction_array(fraction frac_array[], int n) {
  fraction sum = frac_array[0];
  for (int i = 1; i < n; i++) {
    sum = add_fractions(sum, frac_array[i]);
  }
  return sum;
}

void fill_fraction_array(fraction frac_array[], int n) {
  fraction temp_frac;
  temp_frac.num = 1;
  for (int i = 1; i <= n; i++) {
    temp_frac.denom = i * (i + 1);
    frac_array[i - 1] = temp_frac;
  }
}
