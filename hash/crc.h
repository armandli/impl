#include <cstddef>
#include <cstdint>
/* Cyclic Redundancy Check Hashing Algorithm
 */

constexpr size_t CRC_TABLE_SIZE = 256;
uint32_t crctable[CRC_TABLE_SIZE];

void generate_table(){
  crctable[0] = 0U;
  uint32_t crc = 1U;
  size_t i = 128U;
  do {
    if (crc & 0x1U)
      crc = (crc >> 1U) ^ 0x8408U;
    else
      crc >>= 0x1U;

    for (size_t j = 0; j < 256U; j += i * 2)
      crctable[i + j] = crc ^ crctable[j];

    i >>= 1U;
  } while (i > 0);
}

uint32_t crc32(const uint8_t* data, size_t len, uint32_t /*seed not used*/ = 0){
  static bool do_generate_table = true;
  if (do_generate_table){
    generate_table();
    do_generate_table = false;
  }
  
  uint32_t hash = 0xFFFFFFFFU;
  for (size_t i = 0; i < len; ++i){
    uint32_t idx = (hash ^ data[i]) & 0xFFU;
    hash = (hash >> 8U) ^ crctable[idx];
  }
  hash ^= 0xFFFFFFFFU;
  return hash;
}
