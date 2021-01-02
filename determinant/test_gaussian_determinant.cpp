#include <gaussian_determinant.h>

#include <gtest/gtest.h>

namespace s = std;

double** create_mtx(size_t n){
  double** mtx = new double*[n];
  for (size_t i = 0; i < n; ++i)
    mtx[i] = new double[n];
  return mtx;
}

TEST(determinant, test1){
  size_t n = 2;
  double** matrix = create_mtx(n);
  matrix[0][0] = 1.;  matrix[0][1] = 0.;
  matrix[1][0] = -1.; matrix[1][1] = 2.;

  double t = determinant(matrix, n);
  EXPECT_DOUBLE_EQ(2., t);

  for (size_t i = 0; i < n; ++i)
    delete[] matrix[i];
  delete[] matrix;
}

TEST(determinant, test2){
  size_t n = 3;
  double** matrix = create_mtx(n);
  matrix[0][0] = 1.;
  matrix[1][1] = 2.;
  matrix[2][2] = 3.;

  double t = determinant(matrix, n);
  EXPECT_DOUBLE_EQ(6., t);

  for (size_t i = 0; i < n; ++i)
    delete[] matrix[i];
  delete[] matrix;
}
