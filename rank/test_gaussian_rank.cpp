#include <gaussian_rank.h>

#include <gtest/gtest.h>

namespace s = std;

double** create_mtx(size_t nrow, size_t ncol){
  double** mtx = new double*[nrow];
  for (size_t i = 0; i < nrow; ++i)
    mtx[i] = new double[ncol];
  return mtx;
}

TEST(GaussianRank, test1){
  size_t n = 3, m = 3;
  double** matrix = create_mtx(n, m);
  matrix[0][0] = 1.;  matrix[0][1] = 0.;  matrix[0][2] = 1.;
  matrix[1][0] = -2.; matrix[1][1] = -3.; matrix[1][2] = 1.;
  matrix[2][0] = 3.;  matrix[2][1] = 3.;  matrix[2][2] = 0.;

  size_t t = rank(matrix, n, m);

  EXPECT_EQ(2, t);

  for (size_t i = 0; i < n; ++i)
    delete[] matrix[i];
  delete[] matrix;
}

TEST(GaussianRank, test2){
  size_t n = 2, m = 4;
  double** matrix = create_mtx(n, m);
  matrix[0][0] = 1.;  matrix[0][1] = 1.;  matrix[0][2] = 0.; matrix[0][3] = 2.;
  matrix[1][0] = -1.; matrix[1][1] = -1.; matrix[1][2] = 0.; matrix[1][3] = -2.;

  size_t t = rank(matrix, n, m);

  EXPECT_EQ(1, t);

  for (size_t i = 0; i < n; ++i)
    delete[] matrix[i];
  delete[] matrix;
}

TEST(GaussianRank, test3){
  size_t n = 4, m = 2;
  double** matrix = create_mtx(n, m);
  matrix[0][0] = 1.;  matrix[0][1] = -1.;
  matrix[1][0] = 1.;  matrix[1][1] = -1.;
  matrix[2][0] = 0.;  matrix[2][1] = 0.;
  matrix[3][0] = 2.;  matrix[3][1] = -2.;

  size_t t = rank(matrix, n, m);

  EXPECT_EQ(1, t);

  for (size_t i = 0; i < n; ++i)
    delete[] matrix[i];
  delete[] matrix;
}
