#include <cstring>
#include <tuple>
#include <algorithm>

template <typename T, size_t V, typename... Hs>
class CountMinSketch {
  template <size_t Idx>
  constexpr void hash_count(const T& val){
    size_t hash = std::get<Idx>(mHashes)(val);
    mHashIndexes[hash % V + Idx * V]++;
    if constexpr(Idx > 0)
      hash_count<Idx - 1>(val);
  }

  template <size_t Idx>
  constexpr size_t estimate(const T& val){
    if constexpr(Idx > 0){
      size_t hash = std::get<Idx>(mHashes)(val);
      size_t isize = mHashIndexes[hash % V + Idx * V];
      size_t osize = estimate<Idx - 1>(val);
      return std::min(isize, osize);
    } else {
      size_t hash = std::get<Idx>(mHashes)(val);
      return mHashIndexes[hash % V];
    }
  }
public:
  CountMinSketch(Hs... hashes): mHashes(hashes...) {}

  CountMinSketch(const CountMinSketch&) = default;
  CountMinSketch& operator=(const CountMindSketch&) = default;
  ~CountMinSketch() = default;

  void count(const T& v){
    hash_count<sizeof...(Hs) - 1>(v);
  }

  size_t min_estimate(const T& v){
    return estimate<sizeof...(Hs) - 1>(v);
  }

  void clear(){
    memset(mHashIndexes, 0, sizeof(mHashIndexes));
  }
private:
  std::tuple<Hs...> mHashes;
  size_t            mHashIndexes[V * sizeof...(Hs)];
};
