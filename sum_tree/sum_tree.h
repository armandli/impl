#include <cassert>
#include <cstdint>
#include <vector>
#include <algorithm>

#include <iostream>

namespace s = std;
using uint = uint32_t;

class SumTree {
  s::vector<float> mSums;
  s::vector<float> mLeafs;
  uint             mModPadding;
  float            mMax;

protected:
  static uint nearest_power(uint size){
    uint v = size - 1;
    v |= v >> 1U;
    v |= v >> 2U;
    v |= v >> 4U;
    v |= v >> 8U;
    v |= v >> 16U;
    v++;
    return v;
  }

  size_t leaf_size() const {
    return mLeafs.size();
  }

  uint mod_padding() const {
    return mModPadding;
  }

  void init_tree(float init_val){
    mSums.resize(nearest_power(leaf_size()) - 1); //upper bound size
    class Recursion {
      s::vector<float>& sums;
      float             init_val;
    public:
      Recursion(s::vector<float>& sums, float init_val): sums(sums), init_val(init_val) {}
      uint operator()(uint size, uint pos){
        uint first_half, second_half;
        switch (size){
          case 0: case 1: return 0U;
          case 2:
            first_half = second_half = 1U;
            break;
          default: {
            uint half = nearest_power(size) >> 1U;
            uint quarter = half >> 1U;
            first_half = s::min(quarter, size - half) + quarter;
            second_half = size - first_half;
            break;
          }
        }

        sums[pos] = (float)first_half * init_val;

        uint s1 = (*this)(first_half, pos * 2 + 1);
        uint s2 = (*this)(second_half, pos * 2 + 2);
        uint sz = s::max(s1, s2);
        return s::max(pos + 1, sz);
      }
    } recursion(mSums, init_val);
    uint true_size = recursion(leaf_size(), 0);
    mSums.resize(true_size);
  }

  void init_mod_padding(){
    uint idx = 0;
    while (idx < mSums.size())
      idx = idx * 2 + 1;
    mModPadding = leaf_size() - idx % leaf_size();
  }

  uint leaf_idx_to_array_idx(uint leaf_idx) const {
    return (leaf_idx + mModPadding) % leaf_size();
  }
  uint array_idx_to_leaf_idx(uint array_idx) const {
    uint padding = (array_idx + 1 < mModPadding) ? 0U : leaf_size();
    return nearest_power(leaf_size()) - 1 + array_idx - padding;
  }
public:
  SumTree(uint size, float init_val):
    mLeafs(size, init_val), mModPadding(0U), mMax((float)size * init_val){
    assert(init_val > 0.F);
    init_tree(init_val);
    init_mod_padding();
  }
  void update(uint array_idx, float nval){
    assert(array_idx < leaf_size());

    uint leaf_idx = array_idx_to_leaf_idx(array_idx);
    float diff = nval - mLeafs[array_idx];
    mLeafs[array_idx] = nval;
    do {
      if (leaf_idx & 1){
        leaf_idx = (leaf_idx - 1) >> 1U;
        mSums[leaf_idx] += diff;
      } else {
        leaf_idx = (leaf_idx - 2) >> 1U;
      }
    } while (leaf_idx > 0U);
    mMax += diff;
  }
  uint sample(float value) const {
    assert(value < mMax);

    uint idx = 0;
    while (idx < mSums.size())
      if (value < mSums[idx])
        idx = idx * 2 + 1;
      else {
        value -= mSums[idx];
        idx = idx * 2 + 2;
      }
    return leaf_idx_to_array_idx(idx);
  }
  float max() const { return mMax; }
  uint size() const { return leaf_size(); }
};
