#include <gtest/gtest.h>

#include <dft.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <complex>

namespace s = std;

using cmplx = s::complex<double>;

TEST(dft, TestDFT1){
  s::vector<cmplx> a = {5., 0., -5., 0.};

  s::vector<cmplx> y = dft(a);
  s::vector<cmplx> ra = idft(y);

  ASSERT_EQ(a.size(), ra.size());

  for (size_t i = 0; i < a.size(); ++i){
    EXPECT_NEAR(a[i].real(), ra[i].real(), EPSILON);
  }
}
