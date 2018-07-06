#include <cassert>
#include <cstddef>
#include <cstdint>

// Murmur3 Hash Function

uint32_t murmur3(const uint8_t* key, size_t len, uint32_t seed = 0){
  uint32_t hash = seed;
  size_t i = 0;
  const uint32_t* key_x4 = (const uint32_t*)key;
  if (len > 3UL){
    size_t len_x4 = len >> 2U << 2U;
    for (; i < len_x4; i += 4, ++key_x4){
      uint32_t k = *key_x4;
      k *= 0xCC9E2D51U;
      k = (k << 15U) | (k >> 17U);
      k *= 0x1B873593U;
      hash ^= k;
      hash = (hash << 13U) | (hash >> 19U);
      hash = (hash * 5U) + 0xE6546B64U;
    }
  }
  if (len & 3UL){
    uint32_t k = *key_x4;
    switch (len & 3UL){
    case 3U: k &= 0xFFFFFFU; break;
    case 2U: k &= 0xFFFFU;   break;
    case 1U: k &= 0xFFU;     break;
    default: assert(false);
    }
    k *= 0xCC9E2D51U;
    k = (k << 15U) | (k >> 17U);
    k *= 0x1B873593U;
    hash ^= k;
  }
  hash ^= len;
  hash ^= hash >> 16U;
  hash *= 0x85EBCA6BU;
  hash ^= hash >> 13U;
  hash *= 0xC2B2AE35U;
  hash ^= hash >> 16U;
  return hash;
}
