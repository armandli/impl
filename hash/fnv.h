#include <cassert>
#include <cstdint>
#include <cstddef>
// Fowler-Noll-Vo hash functions

constexpr uint64_t fnv_basis64 = 0xCBF29CE484222325ULL;
constexpr uint32_t fnv_basis32 = 0x811C9DC5U;

/* FNV Prime is the smallest prime p that is of the form 256^t + 2^8 + b,
 * such that 0 < b < 2^8, number of one-bits in the binary number representation of b is either 4 or 5, 
 * p mod (2^40 - 2^24 - 1) > (2^24 + 2^8 + 2^7)
 * where t is floor(5 + 2^S / 12) and 4 < S < 11
 * FNV primes matching the above characteristics tend to have better dispersion properties.
 */
constexpr uint64_t fnv_prime64 = 0x100000001B3ULL;
constexpr uint32_t fnv_prime32 = 0x1000193U;

uint64_t fnv1_64(const uint8_t* data, size_t len){
  const uint64_t* data64 = (const uint64_t*)data;
  size_t len64 = (len & (~0x7ull)) / 8;
  uint64_t hash = fnv_basis64;
  size_t i = 0;
  for (; i < len64; ++i){
    hash *= fnv_prime64;
    hash ^= data64[i];
  }
  if (len & 0x7ull){
    uint64_t k = data64[i];
    switch (len & 0x7ull){
    case 7u: k &= 0xffffffffffffffull; break;
    case 6u: k &= 0xffffffffffffull; break;
    case 5u: k &= 0xffffffffffull; break;
    case 4u: k &= 0xffffffffull; break;
    case 3u: k &= 0xffffffull; break;
    case 2u: k &= 0xffffull; break;
    case 1u: k &= 0xffull; break;
    default: assert(false); break;
    }
    hash *= fnv_prime64;
    hash ^= k;
  }
  return hash;
}

uint32_t fnv1_32(const uint8_t* data, size_t len){
  const uint32_t* data32 = (const uint32_t*)data;
  size_t len32 = (len & (~0x3ULL)) / 4;
  uint64_t hash = fnv_basis32;
  size_t i = 0;
  for (; i < len32; ++i){
    hash *= fnv_prime32;
    hash ^= data32[i];
  }
  if (len & 0x3ULL){
    uint32_t k = data32[i];
    switch (len & 0x3ULL){
    case 3U: k &= 0xFFFFFFU; break;
    case 2U: k &= 0xFFFFU; break;
    case 1U: k &= 0xFFU; break;
    default: assert(false); break;
    }
    hash *= fnv_prime32;
    hash ^= k;
  }
  return hash;
}

uint64_t fnv1a_64(const uint8_t* data, size_t len){
  const uint64_t* data64 = (const uint64_t*)data;
  size_t len64 = (len & (~0x7ull)) / 8;
  uint64_t hash = fnv_basis64;
  size_t i = 0;
  for (; i < len64; ++i){
    hash ^= data64[i];
    hash *= fnv_prime64;
  }
  if (len & 0x7ull){
    uint64_t k = data64[i];
    switch (len & 0x7ull){
    case 7u: k &= 0xffffffffffffffull; break;
    case 6u: k &= 0xffffffffffffull; break;
    case 5u: k &= 0xffffffffffull; break;
    case 4u: k &= 0xffffffffull; break;
    case 3u: k &= 0xffffffull; break;
    case 2u: k &= 0xffffull; break;
    case 1u: k &= 0xffull; break;
    default: assert(false); break;
    }
    hash ^= k;
    hash *= fnv_prime64;
  }
  return hash;
}

uint32_t fnv1a_32(const uint8_t* data, size_t len){
  const uint32_t* data32 = (const uint32_t*)data;
  size_t len32 = (len & (~0x3ULL)) / 4;
  uint64_t hash = fnv_basis32;
  size_t i = 0;
  for (; i < len32; ++i){
    hash ^= data32[i];
    hash *= fnv_prime32;
  }
  if (len & 0x3ULL){
    uint32_t k = data32[i];
    switch (len & 0x3ULL){
    case 3U: k &= 0xFFFFFFU; break;
    case 2U: k &= 0xFFFFU; break;
    case 1U: k &= 0xFFU; break;
    default: assert(false); break;
    }
    hash ^= k;
    hash *= fnv_prime32;
  }
  return hash;
}
