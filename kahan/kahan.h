/* Kahan Summation:
 *   summing floating point values of limited precision and minimize loss of precision in result
 */
#include <type_traits>

template <template <typename> Container, typename T,
          bool isFloatingPointType = std::is_floating_point<T>::value>
T kahansum(const Container<T>& input){
  static_assert(isFloatingPointType,
                "Kahan sum should apply to floating point type");

  T sum = 0.L, c = 0.L;
  for (auto num : input){
    T y = num - c;
    T t = sum + y;
    c   = (t - sum) - y;
    sum = t;
    }
  return sum;
  }
