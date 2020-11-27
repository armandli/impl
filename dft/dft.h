#include <vector>
#include <complex>

namespace s = std;

using cmplx = s::complex<double>;

constexpr double M_PI = 3.1415926;
constexpr double TWO_PI = M_PI * 2.;
constexpr double NTWO_PI = M_PI * -2.;

s::vector<cmplx> dft(const s::vector<cmplx>& a){
  double n = a.size();
  s::vector<cmplx> y;
  y.reserve(a.size());

  for (size_t k = 0; k < a.size(); ++k){
    cmplx x = 0.;
    for (size_t i = 0; i < a.size(); ++i){
      double w = NTWO_PI * (double)k * (double)i / n;
      cmplx c(cos(w), sin(w));
      x += c * a[i];
    }
    y.push_back(x / n);
  }
  return y;
}

s::vector<cmplx> idft(const s::vector<cmplx>& y){
  double n = y.size();
  s::vector<cmplx> a;
  a.reserve(y.size());

  for (size_t k = 0; k < y.size(); ++k){
    cmplx x = 0.;
    for (size_t i = 0; i < y.size(); ++i){
      double w = TWO_PI * (double)k * (double)i / n;
      cmplx c(cos(w), sin(w));
      x += c * a[i];
    }
    a.push_back(x);
  }
  return a;
}
