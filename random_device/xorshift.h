#include <cstdint>
#include <random>

class xorshift {
public:
  using result_type = uint32_t;
  static constexpr result_type (min)() { return 0; }
  static constexpr result_type (max)() { return UINT32_MAX; }

  xorshift() : mSeed(0xC1F651C67C62C6E0ULL) {}
  explicit xorshift(std::random_device &rd){
    seed(rd);
  }
  template <typename T>
  explicit xorshift(const T& s): mSeed(s) {}

  void seed(std::random_device &rd){
    mSeed = uint64_t(rd()) << 31 | uint64_t(rd());
  }

  result_type operator()(){
    uint64_t result = mSeed * 0xD989BCACC137DCD5ULL;
    mSeed ^= mSeed >> 11ULL;
    mSeed ^= mSeed << 31ULL;
    mSeed ^= mSeed >> 18ULL;
    return uint32_t(result >> 32ULL);
  }

  void discard(unsigned long long n){
    for (unsigned long long i = 0; i < n; ++i)
      operator()();
  }

  bool operator==(const xorshift& o) const {
    return mSeed == o.mSeed;
  }
  bool operator!=(const xorshift& o) const {
    return not this->operator==(o);
  }
private:
  uint64_t mSeed;
};

