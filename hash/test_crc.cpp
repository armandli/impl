#include <crc.h>

#include <cstdio>
#include <string>
#include <gtest/gtest.h>

template <typename T> struct CRCHash;

template <>
struct CRCHash<std::string> {
  size_t operator()(const std::string& s) const {
    const uint8_t* data = (const uint8_t*)s.c_str();
    size_t hash = crc32(data, s.length());
    return hash;
  }
};

template <>
struct CRCHash<int> {
  size_t operator()(const int& v) const {
    const uint8_t* data = (const uint8_t*)&v;
    size_t hash = crc32(data, sizeof(int));
    return hash;
  }
};

TEST(CRCTestString, Test1){
  std::string s("hello world");
  CRCHash<std::string> hash;
  EXPECT_EQ(0xffffe01e, hash(s));
}

TEST(CRCTestInt, Test1){
  int v = 1337;
  CRCHash<int> hash;
  EXPECT_EQ(0xffff804a, hash(v));
}
