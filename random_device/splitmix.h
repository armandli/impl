#include <cstdint>
#include <random>

class splitmix {
public:
  using result_type = uint32_t;
  static constexpr result_type (min)() { return 0; }
  static constexpr result_type (max)() { return UINT32_MAX; }

  splitmix() : mSeed(1) {}
  explicit splitmix(std::random_device &rd){
    seed(rd);
  }
  template <typename T>
  explicit splitmix(const T& s): mSeed(s) {}

  void seed(std::random_device &rd){
    mSeed = uint64_t(rd()) << 31 | uint64_t(rd());
  }

  result_type operator()(){
    uint64_t z = (mSeed += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return result_type((z ^ (z >> 31)) >> 31);
  }

  void discard(unsigned long long n){
    for (unsigned long long i = 0; i < n; ++i)
      operator()();
  }

  bool operator==(const splitmix& o) const {
    return mSeed == o.mSeed;
  }
  bool operator!=(const splitmix& o) const {
    return not this->operator==(o);
  }
private:
  uint64_t mSeed;
};
