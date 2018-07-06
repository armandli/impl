#include <fnv.h>

#include <cstdio>
#include <string>
#include <gtest/gtest.h>

template <typename T> struct FNV1_Hash;

template <>
struct FNV1_Hash<std::string> {
  size_t operator()(const std::string& s) const {
    const uint8_t* data = (const uint8_t*)s.c_str();
    size_t hash = fnv1_32(data, s.length());
    return hash;
  }
};

template <>
struct FNV1_Hash<int> {
  size_t operator()(const int& v) const {
    const uint8_t* data = (const uint8_t*)&v;
    size_t hash = fnv1_32(data, sizeof(int));
    return hash;
  }
};

template <typename T> struct FNV1A_Hash;

template <>
struct FNV1A_Hash<std::string> {
  size_t operator()(const std::string& s) const {
    const uint8_t* data = (const uint8_t*)s.c_str();
    size_t hash = fnv1a_32(data, s.length());
    return hash;
  }
};

template <>
struct FNV1A_Hash<int> {
  size_t operator()(const int& v) const {
    const uint8_t* data = (const uint8_t*)&v;
    size_t hash = fnv1a_32(data, sizeof(int));
    return hash;
  }
};

TEST(FNV1TestString, Test1){
  std::string s("hello world");
  FNV1_Hash<std::string> hash;
  EXPECT_EQ(0x54b4383c, hash(s));
}

TEST(FNV1TestInt, Test1){
  int i = 1337;
  FNV1_Hash<int> hash;
  EXPECT_EQ(0x50c5826, hash(i));
}

TEST(FNV1ATestString, Test1){
  std::string s("hello world");
  FNV1A_Hash<std::string> hash;
  EXPECT_EQ(0xf6e6f1ae, hash(s));
}

TEST(FNV1ATestInt, Test1){
  int i = 1337;
  FNV1A_Hash<int> hash;
  EXPECT_EQ(0x3c04d4b4, hash(i));
}
