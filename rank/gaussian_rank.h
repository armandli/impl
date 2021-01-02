#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>

namespace s = std;

template <typename T> constexpr T EPSILON;
template <> constexpr double EPSILON<double> = 1e-12;
template <> constexpr double EPSILON<float> = 1e-12;

template <typename T>
void normalize(T** mtx, size_t ridx, size_t cidx, size_t nrow, size_t ncol){
  for (size_t i = ridx + 1; i < ncol; ++i)
    mtx[ridx][i] /= mtx[ridx][cidx];
  for (size_t i = 0; i < nrow; ++i)
    if (i != ridx && s::abs(mtx[i][cidx]) > EPSILON<T>)
      for (size_t j = cidx + 1; j < ncol; ++j)
        mtx[i][j] -= mtx[ridx][j] * mtx[i][cidx];
}

template <typename T>
size_t rank(T** mtx, size_t nrow, size_t ncol){
  size_t rank = 0;
  s::vector<bool> row_selected(nrow, false);
  for (size_t j = 0; j < ncol; ++j){
    size_t i;
    //select a row with pivot non-zero, we don't need to select the one with max
    //value, and don't need to swap the rows unlike normal gaussian elimination
    for (i = 0; i < nrow; ++i)
      if (not row_selected[i] && s::abs(mtx[i][j]) > EPSILON<T>)
        break;

    //if a row is selected
    if (i < nrow){
      ++rank;
      row_selected[i] = true;
      //normalize the other rows by this selected row
      normalize(mtx, i, j, nrow, ncol);
    }
  }

  return rank;
}
