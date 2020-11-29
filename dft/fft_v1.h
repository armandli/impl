#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

namespace s = std;

using cmplx = s::complex<double>;

constexpr double TWO_PI = M_PI * 2.;
constexpr double NTWO_PI = M_PI * -2.;
constexpr double EPSILON = 1e-15;

// naive recursive implementation of FFT in O(n log n)

// assume size of a is a power of 2
s::vector<cmplx> fft(const s::vector<cmplx>& a){
  // we should never reach length of 0
  assert(a.size() > 0);

  s::vector<cmplx> y; y.reserve(a.size());

  //base case, if n == 1, e^i0 = 1, y = a * 1
  if (a.size() == 1){
    y.push_back(a.back());
    return y;
  }

  size_t half_size = a.size() >> 1;

  s::vector<cmplx> a0; a0.reserve(half_size);
  s::vector<cmplx> a1; a1.reserve(half_size);
  for (size_t i = 0; i < half_size; i++){
    a0.push_back(a[i * 2]);
    a1.push_back(a[i * 2 + 1]);
  }

  s::vector<cmplx> y0 = fft(a0);
  s::vector<cmplx> y1 = fft(a1);

  assert(y0.size() == half_size);
  assert(y1.size() == half_size);

  double n = a.size();
  cmplx w(1.), wn(cos(NTWO_PI / n), sin(NTWO_PI / n));
  for (size_t i = 0; i < half_size; ++i){
    y.push_back((y0[i] + w * y1[i]) / 2.);
    w *= wn;
  }
  w = 1.;
  for (size_t i = 0; i < half_size; ++i){
    y.push_back((y0[i] - w * y1[i]) / 2.);
    w *= wn;
  }
  return y;
}

// assume size of y is a power of 2
s::vector<cmplx> ifft(const s::vector<cmplx>& y){
  // we should never reach length of 0
  assert(y.size() > 0);

  s::vector<cmplx> a; a.reserve(y.size());

  //base case, if n == 1, e^i0 = 1, y = a * 1
  if (y.size() == 1){
    a.push_back(y.back());
    return a;
  }

  size_t half_size = y.size() >> 1;

  s::vector<cmplx> y0; y0.reserve(half_size);
  s::vector<cmplx> y1; y1.reserve(half_size);
  for (size_t i = 0; i < half_size; i++){
    y0.push_back(y[i * 2]);
    y1.push_back(y[i * 2 + 1]);
  }

  s::vector<cmplx> a0 = ifft(y0);
  s::vector<cmplx> a1 = ifft(y1);

  assert(a0.size() == half_size);
  assert(a1.size() == half_size);

  double n = y.size();
  cmplx w(1.), wn(cos(TWO_PI / n), sin(TWO_PI / n));
  for (size_t i = 0; i < half_size; ++i){
    a.push_back(a0[i] + w * a1[i]);
    w *= wn;
  }
  w = 1.;
  for (size_t i = 0; i < half_size; ++i){
    a.push_back(a0[i] - w * a1[i]);
    w *= wn;
  }
  return a;
}
