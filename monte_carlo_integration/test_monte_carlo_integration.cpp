#include <monte_carlo_integration.h>

#include <cmath>
#include <array>
#include <iostream>

#include <gtest/gtest.h>

double func_sin_sq(const std::array<double, 1>& s){
  double ss = std::sin(s[0]);
  return ss * ss;
}

TEST(MonteCarloIntegration, TestSinSquare){
  double t = unbiased_monte_carlo_integration(func_sin_sq, (std::array<std::pair<double, double>, 1>){(std::pair<double, double>){0., 3.14}}, 10000);
  double r = M_PI / 2.;

  std::cout << "real result: " << r << " monte carlo: " << t << std::endl;
}
