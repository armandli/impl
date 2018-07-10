#include <cassert>

constexpr bool is_power_of_two(size_t b){
  return (b & (b - 1)) == 0;
}

constexpr size_t number_of_bits(size_t b){
  size_t ret = 0; 
  size_t u = b - 1;
  while (u){
    ret += 1;
    u = u & (u - 1);
  }
  return ret;
}

constexpr double GoldenRatio = 1.6180339;
constexpr unsigned long long Two64DivPhi = 11400714819323198485ULL;

//fibonacci hash does not share the same common interface as other hash functions

// fibonacci hashing is faster and in general better than doing integer modulo when hashing integer values
uint64_t fibonacci_hash(unsigned long long v, size_t buckets){
  if (buckets < 2 || not is_power_of_two(buckets)) assert("Number of buckets must be power of two");
  return (v * Two64DivPhi) >> (64U - number_of_bits(buckets));
}

uint64_t fibonacci_hash_opt(unsigned long long v, size_t buckets){
  if (buckets < 2 || not is_power_of_two(buckets)) assert("Number of buckets must be power of two");
  size_t shift_amount = 64U - number_of_bits(buckets);
  v ^= v >> shift_amount;
  return (v * Two64DivPhi) >> shift_amount;
}
