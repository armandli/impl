#include <cassert>
#include <random>
#include <algorithm>

// monte carlo integration is used to estimate the area of a integral without having to
// derive the integral formula; result of monte carlo integration is often close enough
// to the real integral value

// concept of Monte Carlo Integration is simple: we take random number between the 
// range we want to integrate with uniformly (can also be non-uniform), and compute the Y
// value from function F, for a large enough sample S. We then average the Y by the number
// of samples S, and multiply it with the integral range e - s
template <typename F>
double monte_carlo_integration(F f, double s, double e, size_t ns){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(std::min(s, e), std::max(s, e));

  double sum = 0.;
  for (size_t i = 0; i < ns; ++i){
    double x = dist(mt);
    double y = f(x);
    sum += y;
  }
  double avg = sum / double(ns);
  return avg * std::abs(e - s);
}
