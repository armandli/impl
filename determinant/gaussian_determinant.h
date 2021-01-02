#include <cassert>
#include <cmath>
#include <algorithm>

namespace s = std;

template <typename T> constexpr T EPSILON;
template <> constexpr double EPSILON<double> = 1e-12;
template <> constexpr double EPSILON<float> = 1e-7;

template <typename T>
size_t max_row_idx(T** mtx, size_t cidx, size_t nrow){
  size_t maxr = cidx;
  for (size_t i = cidx + 1; i < nrow; ++i)
    if (s::abs(mtx[i][cidx]) > s::abs(mtx[maxr][cidx]))
      maxr = i;
  return maxr;
}

template <typename T>
void swap_row(T** mtx, size_t ridx1, size_t ridx2, size_t ncol){
  for (size_t i = 0; i < ncol; ++i){
    T t = mtx[ridx1][i];
    mtx[ridx1][i] = mtx[ridx2][i];
    mtx[ridx2][i] = t;
  }
}

template <typename T>
void normalize(T** mtx, size_t didx, size_t n){
  for (size_t i = didx + 1; i < n; ++i)
    mtx[didx][i] /= mtx[didx][didx];
  for (size_t i = 0; i < n; ++i){
    if (i != didx && s::abs(mtx[i][didx]) > EPSILON<T>)
      for (size_t j = didx + 1; j < n; ++j)
        mtx[i][j] -= mtx[didx][j] * mtx[i][didx];
  }
}

template <typename T>
T determinant(T** mtx, size_t n){
  T det = 1.;
  for (size_t r = 0; r < n; ++r){
    size_t max_row = max_row_idx(mtx, r, n);
    if (mtx[max_row][r] < EPSILON<T>)
      return 0.;
    if (max_row != r){
      swap_row(mtx, max_row, r, n);
      det *= -1.;
    }
    det *= mtx[r][r];
    normalize(mtx, r, n);
  }

  return det;
}
