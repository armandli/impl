#include <cassert>
#include <cmath>
#include <algorithm>

namespace s = std;

template <typename T> constexpr T EPSILON;
template <> constexpr double EPSILON<double> = 1e-12;
template <> constexpr float EPSILON<float> = 1e-6;

template <typename T>
size_t max_row_idx(T** mtx, size_t cidx, size_t nrow){
  size_t maxr = cidx;
  for (size_t i = cidx; i < nrow; ++i)
    if (abs(mtx[i][cidx]) > abs(mtx[maxr][cidx]))
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
void zero_column_by_row(T** mtx, size_t ridx, size_t cidx, size_t nrow, size_t ncol){
  // does not zero rows before ridx to make upper triangular matrix
  for (size_t i = ridx + 1; i < nrow; ++i){
    double ratio = mtx[i][cidx] / mtx[ridx][cidx];
    // assume columns before cidx are already 0
    for (size_t j = cidx; j < ncol; ++j)
      mtx[i][j] -= mtx[ridx][j] * ratio;
  }
}

enum class NUM_SOLUTIONS: size_t {
  ZERO,
  ONE,
  INF,
};

template <typename T>
NUM_SOLUTIONS gaussian_elimination(T* x, T** mtx, size_t nrow, size_t ncol){
  int* pivots = new int[ncol - 1];
  s::fill(pivots, &pivots[ncol - 1], -1);
  for (size_t r = 0, c = 0; c < ncol - 1 && r < nrow; ++c){
    size_t max_row = max_row_idx(mtx, r, nrow);
    if (abs(mtx[max_row][c]) < EPSILON<T>) continue;
    swap_row(mtx, max_row, r, ncol);
    zero_column_by_row(mtx, r, c, nrow, ncol); // create upper triangular matrix
    pivots[c] = r;

    ++r;
  }

  s::fill(x, &x[ncol-1], 0.);
  NUM_SOLUTIONS s = NUM_SOLUTIONS::ONE;
  for (int c = ncol - 2; c >= 0; --c){
    if (pivots[c] == -1){
      s = NUM_SOLUTIONS::INF;
      continue;
    }
    T sum = 0.;
    for (size_t j = c + 1; j < ncol - 1; ++j){
      sum += x[j] * mtx[pivots[c]][j];
    }
    x[c] = (mtx[pivots[c]][ncol - 1] - sum) / mtx[pivots[c]][c];
  }
  delete[] pivots;

  // check if solution is available
  for (size_t i = 0; i < nrow; ++i){
    T sum = 0.;
    for (size_t j = 0; j < ncol - 1; ++j)
      sum += mtx[i][j] * x[j];
    if (abs(sum - mtx[i][ncol - 1]) > EPSILON<T>)
      s = NUM_SOLUTIONS::ZERO;
  }

  return s;
}
