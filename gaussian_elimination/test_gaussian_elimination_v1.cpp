#include <gtest/gtest.h>

#include <iostream>
#include <gaussian_elimination_v1.h>

namespace s = std;

double** create_mtx(size_t nrow, size_t ncol){
  double** mtx = new double*[nrow];
  for (size_t i = 0; i < nrow; ++i)
    mtx[i] = new double[ncol];
  return mtx;
}

TEST(Test, GaussianElimination1){
  size_t nrow = 3, ncol = 4;
  double** matrix = create_mtx(nrow, ncol);
  matrix[0][0] = 2.; matrix[0][1] = 1.;  matrix[0][2] = -2.; matrix[0][3] = 3.;
  matrix[1][0] = 1.; matrix[1][1] = -1.; matrix[1][2] = -1.; matrix[1][3] = 0.;
  matrix[2][0] = 1.; matrix[2][1] = 1.;  matrix[2][2] = 3.;  matrix[2][3] = 12.;

  double* x = new double[ncol - 1];
  NUM_SOLUTIONS s = gaussian_elimination(x, matrix, nrow, ncol);

  EXPECT_EQ(NUM_SOLUTIONS::ONE, s);
  EXPECT_DOUBLE_EQ(3.5, x[0]);
  EXPECT_DOUBLE_EQ(1., x[1]);
  EXPECT_DOUBLE_EQ(2.5, x[2]);

  for (size_t i = 0; i < nrow; ++i)
    delete[] matrix[i];
  delete[] matrix;
  delete[] x;
}
