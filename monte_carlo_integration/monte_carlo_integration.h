#include <cassert>
#include <random>
#include <array>

// monte carlo integration is used to estimate the area of a integral without having to
// derive the integral formula; result of monte carlo integration is often close enough
// to the real integral value

// concept of Monte Carlo Integration is simple: we take random number between the 
// range we want to integrate with uniformly (can also be non-uniform), and compute the Y
// value from function F, for a large enough sample S. We then average the Y by the number
// of samples S, and multiply it with the integral range e - s

template <typename F, size_t P>
double unbiased_monte_carlo_integration(F f, const std::array<std::pair<double, double>, P>& bounds, size_t ns){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::array<std::uniform_real_distribution<double>, P> dists;
  for (size_t i = 0; i < P; ++i)
    dists[i] = std::uniform_real_distribution<double>(bounds[i].first, bounds[i].second);

  double sum = 0.;
  for (size_t i = 0; i < ns; ++i){
    std::array<double, P> Xs;
    for (size_t j = 0; j < P; ++j)
      Xs[j] = dists[j](mt);
    double y = f(Xs);
    sum += y;
  }
  double avg = sum / double(ns);
  for (size_t i = 0; i < P; ++i)
    avg *= bounds[i].second - bounds[i].first;
  return avg;
}

//variance reduction monte carlo
//monte carlo works even on non-uniform distribution,
//this is useful if the the probability distribution match the shape of the function,
//it would reduce the variance and allow monte carlo to converge faster.
//the algorithm is now computing:
//  Avg_N = Sigma(F(X) / pdf(X)) for N 
template <typename F, typename ICDF, typename PDF, size_t P>
double vr_monte_carlo_integration(F f, const std::array<ICDF, P>& icdfs, const std::array<PDF, P>& pdfs, size_t ns){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0., 1.);

  double sum = 0.;
  for (size_t i = 0; i < ns; ++i){
    std::array<double, P> Xs;
    double pdf = 1.;
    for (size_t j = 0; j < P; ++j){
      Xs[j] = icdf[j](dist(mt));
      pdf *= pdfs[j](Xs[j]);
    }
    double y = f(Xs);
    sum += y / pdf;
  }
  return sum / (double)ns;
}

// quasi monte carlo
// use a number of randomly generated points along each input dimension conforming to a regularly spaced grid.
