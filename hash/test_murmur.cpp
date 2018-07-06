#include <murmur.h>

#include <cstdio>
#include <string>
#include <gtest/gtest.h>

template <typename T> class MurmurHash;

template <>
class MurmurHash<std::string> {
public:
  size_t operator()(const std::string& v){
    const uint8_t* data = (const uint8_t*)v.c_str();
    size_t hv = murmur3(data, v.length());
    return hv;
  }
};

template <>
class MurmurHash<int> {
public:
  size_t operator()(const int& v){
    const uint8_t* data = (const uint8_t*)&v;
    size_t hv = murmur3(data, sizeof(int));
    return hv;
  }
};

TEST(HashString, Test1){
  std::string s("hello world");
  MurmurHash<std::string> hash;
  EXPECT_EQ(0x5e928f0f, hash(s));
}

TEST(HashInt, Test1){
  int v = 1337;
  MurmurHash<int> hash;
  EXPECT_EQ(0xcb6c191f, hash(v));
}
