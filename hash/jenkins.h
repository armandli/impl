#include <cstdint>
#include <cstddef>
// Jenkins Hash Function

uint32_t jenkins(const uint8_t* key, size_t len, uint32_t seed = 0U){
  uint32_t hash = seed;
  for (size_t i = 0; i < len; ++i){
    hash += key[i];
    hash += hash << 10U;
    hash ^= hash >> 6U;
  }
  hash += hash << 3U;
  hash ^= hash >> 11U;
  hash += hash << 15U;
  return hash;
}
