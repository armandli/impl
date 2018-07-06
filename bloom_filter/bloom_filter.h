#include <cstring>
#include <tuple>

/* Probability of False Positive:
 * let m be the size of the bit array, k be the number of hash functions, n be the number of expected elements:
 *
 * P = (1 - [1 - 1 / m]^kn)^k
 *
 * if expected number of elements n is known, desired false positive probability is P then the size of the array
 * m can be calculated using:
 *
 * m = - n ln(P) / (ln(2))^2
 *
 * if m is size of array and n is number of elements, then k must be
 *
 * k = m / 2 * ln(2)
 */

template <typename T, size_t V, typename... Hs>
class BloomFilter {
  template <size_t Idx>
  constexpr void set_idxes(const T& val){
    size_t hash = std::get<Idx>(mHashes)(val);
    mHashIndexes[hash % V] = true;
    if constexpr(Idx > 0)
      set_idxes<Idx - 1>(val);
  }

  template <size_t Idx>
  constexpr bool test_hash(const T& val) const {
    size_t hash = std::get<Idx>(mHashes)(val);
    if (mHashIndexes[hash % V] == false) return false;
    if constexpr(Idx > 0) return test_hash<Idx - 1>(val);
    else                  return true;
  }
public:
  BloomFilter(Hs... hashes): mHashes(hashes ...) {}

  BloomFilter(const BloomFilter& o) = default;
  BloomFilter& operator=(const BloomFilter& o) = default;
  ~BloomFilter() = default;

  void add(const T& v){
    set_idxes<sizeof...(Hs) - 1>(v);
    mCount++;
  }

  bool test(const T& v) const {
    return test_hash<sizeof...(Hs) - 1>(v);
  }

  size_t count() const {
    return mCount;
  }

  void clear(){
    memset(mHashIndexes, 0, sizeof(mHashIndexes));
    mCount = 0;
  }
private:
  std::tuple<Hs...> mHashes;
  bool              mHashIndexes[V];
  size_t            mCount;
};
