#include <cassert>
#include <cmath>

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
  for (size_t i = 0; i < nrow; ++i){
    if (i == ridx) continue;
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
  // convert to diagonal matrix
  for (size_t r = 0, c = 0; c < ncol - 1 && r < nrow; ++c){
    size_t max_row = max_row_idx(mtx, r, nrow);
    if (abs(mtx[max_row][c]) < EPSILON<T>) continue;
    swap_row(mtx, max_row, r, ncol);
    zero_column_by_row(mtx, r, c, nrow, ncol);

    ++r;
  }

  for (size_t i = 0; i < ncol - 1; ++i)
    x[i] = 0.;
  size_t min_size = nrow < ncol - 1 ? nrow : ncol - 1;
  NUM_SOLUTIONS s = min_size < ncol - 1 ? NUM_SOLUTIONS::INF : NUM_SOLUTIONS::ONE;
  for (size_t r = 0, c = 0; c < min_size; ++c){
    if (abs(mtx[r][c]) < EPSILON<T>){
      s = NUM_SOLUTIONS::INF;
      continue;
    }
    x[c] = mtx[r][ncol - 1] / mtx[r][c];

    ++r;
  }

  //check if solution is unsolvable
  for (size_t i = 0; i < nrow; ++i){
    T sum = 0.;
    for (size_t j = 0; j < ncol - 1; ++j)
      sum += mtx[i][j] * x[j];
    if (abs(sum - mtx[i][ncol - 1]) > EPSILON<T>)
      s = NUM_SOLUTIONS::ZERO;
  }

  return s;
}
