#include <type_traits>

// reverse a integer type in O(log n) time
template <typename T, bool Cond = std::is_integral<T>::value>
constexpr T bit_reverse(T num){
  static_assert(Cond, "Parameter must be an integral type");
  T s = 8 * sizeof(num);
  T mask = (T)~0ULL;
  while ((s >>= 1) > 0){
    mask ^= (mask << s);
    num = ((num >> s) & mask) | ((num << s) & ~mask);
  }
  return num;
}
