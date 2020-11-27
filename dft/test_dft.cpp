#include <gtest/gtest.h>

#include <dft.h>
#include <iostream>
#include <vector>
#include <complex>

namespace s = std;

using cmplx = s::complex<double>;

TEST(dft, TestDFT1){
  s::vector<cmplx> a = {5., 0., -5., 0.};

  s::vector<cmplx> y = dft(a);
  s::vector<cmplx> ra = idft(y);

  for (cmplx c : ra){
    s::cout << c << s::endl;
  }
}
