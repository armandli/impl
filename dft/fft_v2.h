#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

namespace s = std;

using cmplx = s::complex<double>;

constexpr double TWO_PI = M_PI * 2.;
constexpr double NTWO_PI = M_PI * -2.;
constexpr double EPSILON = 1e-15;

// optimize FFT implementation by avoiding memory allocation

// assume size of v is a power of 2
s::vector<cmplx> bit_reversal_permute(const s::vector<cmplx>& v){
  assert(v.size() >= 1);

  s::vector<size_t> idx; idx.reserve(v.size());
  idx.push_back(0);

  // generate the permutation sequence
  while (idx.size() < v.size()){
    size_t n = idx.size();
    for (size_t i = 0; i < n; ++i)
      idx[i] *= 2;
    for (size_t i = 0; i < n; ++i)
      idx.push_back(idx[i] + 1);
  }

  s::vector<cmplx> r; r.reserve(v.size());
  for (size_t i = 0; i < idx.size(); ++i){
    r.push_back(v[idx[i]]);
  }
  return r;
}

// assume size of a is a power of 2
s::vector<cmplx> fft(const s::vector<cmplx>& a){
  s::vector<cmplx> y = bit_reversal_permute(a);

  for (size_t len = 2; len <= y.size(); len <<= 1){
    size_t half_len = len >> 1;
    cmplx wn(cos(NTWO_PI / (double)len), sin(NTWO_PI / (double)len));
    for (size_t i = 0; i < y.size(); i += len){
      cmplx w(1.);
      for (size_t j = i; j < i + half_len; ++j){
        cmplx bp = y[j] + w * y[j + half_len];
        cmplx bn = y[j] - w * y[j + half_len];
        y[j]            = bp;
        y[j + half_len] = bn;
        w *= wn;
      }
    }
  }

  double n = y.size();
  for (cmplx& c : y)
    c /= n;
  return y;
}

// assume size of y is a power of 2
s::vector<cmplx> ifft(const s::vector<cmplx>& y){
  s::vector<cmplx> a = bit_reversal_permute(y);

  for (size_t len = 2; len <= a.size(); len <<= 1){
    size_t half_len = len >> 1;
    cmplx wn(cos(TWO_PI / (double)len), sin(TWO_PI / (double)len));
    for (size_t i = 0; i < a.size(); i += len){
      cmplx w(1.);
      for (size_t j = i; j < i + half_len; ++j){
        cmplx bp = a[j] + w * a[j + half_len];
        cmplx bn = a[j] - w * a[j + half_len];
        a[j]            = bp;
        a[j + half_len] = bn;
        w *= wn;
      }
    }
  }

  return a;
}
