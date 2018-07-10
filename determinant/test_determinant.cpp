#include <matrix.h>

#include <gtest/gtest.h>

TEST(determinant, test1){
  Mtx<double> m(2, 2);
  m.set({1.,  0.,
         -1., 2.});

  double t = det(m);
  EXPECT_DOUBLE_EQ(2., t);
}

TEST(determinant, test2){
  Mtx<double> m(3, 3);
  m.set({1., 0., 0.,
         0., 2., 0.,
         0., 0., 3.});

  double t = det(m);
  EXPECT_DOUBLE_EQ(6., t);
}
