#include <monte_carlo_integration.h>

#include <cmath>
#include <iostream>

#include <gtest/gtest.h>

double func_sin_sq(double s){
  double ss = std::sin(s);
  return ss * ss;
}

TEST(MonteCarloIntegration, TestSinSquared){
  double t = monte_carlo_integration(func_sin_sq, 0., 3.14, 10000);
  double r = M_PI / 2.;

  std::cout << "real result: " << r << " monte carlo: " << t << std::endl;
}
