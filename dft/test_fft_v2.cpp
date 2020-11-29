#include <gtest/gtest.h>

#include <fft_v2.h>
#include <iostream>
#include <vector>
#include <complex>

namespace s = std;

using cmplx = s::complex<double>;

TEST(fft_v1, TestFF1V1_1){
  s::vector<cmplx> a = {5., 0., -5., 0.};

  s::vector<cmplx> y = fft(a);
  s::vector<cmplx> ra = ifft(y);

  ASSERT_EQ(a.size(), ra.size());

  for (size_t i = 0; i < a.size(); ++i){
    EXPECT_NEAR(a[i].real(), ra[i].real(), EPSILON);
  }
}
