#include <cassert>
#include <cstring>
#include <initializer_list>
#include <unordered_set>
#include <algorithm>

template <typename T>
class Mtx {
public:
  Mtx() = default;
  Mtx(size_t r, size_t c): mData(new T[r * c]), mRows(r), mCols(c) {
    memset(mData, 0, sizeof(T) * r * c);
  }
  ~Mtx(){
    if (mData) delete[] mData;
  }

  Mtx(Mtx&& o): mData(o.mData), mRows(o.mRows), mCols(o.mCols) {
    o.mData = nullptr;
    o.mRows = o.mCols = 0;
  }

  Mtx& operator=(Mtx&& o){
    T* tmp = mData;
    mData = o.mData;
    mRows = o.mRows;
    mCols = o.mCols;
    o.mData = nullptr;
    o.mRows = o.mCols = 0;
    if (tmp) delete[] tmp;
    return *this;
  }

  Mtx(const Mtx&) = delete;
  Mtx& operator=(const Mtx&) = delete;

  size_t rows() const {
    return mRows;
  }

  size_t cols() const {
    return mCols;
  }

  T at(size_t r, size_t c) const {
    assert(r * mCols + c < mRows * mCols);
    return mData[r * mCols + c];
  }

  void set(std::initializer_list<T> l){
    assert(l.size() == mRows * mCols);

    std::copy(std::begin(l), std::end(l), mData);
  }
private:
  T* mData;
  size_t mRows;
  size_t mCols;
};

template <typename T>
T det(const Mtx<T>& m){
  assert(m.rows() == m.cols());
  assert(m.rows() > 1 and m.cols() > 1);

  struct DeterminantRecursion {
    const Mtx<T>& m;
    DeterminantRecursion(const Mtx<T>& m): m(m) {}

    T operator()(std::unordered_set<size_t>& idx){
      // base case 2*2 matrix
      if (idx.size() == m.rows() - 2){
        size_t dcols[2];
        for (size_t i = 0, didx = 0; i < m.cols(); ++i)
          if (idx.find(i) == idx.end())
            dcols[didx++] = i;
        return m.at(m.rows() - 2, dcols[0]) * m.at(m.rows() - 1, dcols[1]) - m.at(m.rows() - 2, dcols[1]) * m.at(m.rows() - 1, dcols[0]);
      }

      T sign = 1.;
      T sum = 0.;
      size_t cur_row = idx.size();
      for (size_t i = 0; i < m.cols(); ++i){
        if (idx.find(i) == idx.end()){
          idx.insert(i);
          sum += sign * m.at(cur_row, i) * (*this)(idx);
          idx.erase(i);

          sign *= -1.;
        }
      }
      return sum;
    }
  } recursion(m);

  std::unordered_set<size_t> idx;
  return recursion(idx);
}
