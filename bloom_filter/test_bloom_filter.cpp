#include <bloom_filter.h>

#include <gtest/gtest.h>

#include <string>
#include <fnv.h>
#include <murmur.h>
#include <jenkins.h>

template <typename T> struct FNV1Hash;
template <> struct FNV1Hash<std::string> {
  size_t operator()(const std::string& v) const noexcept {
    const uint8_t* data = (const uint8_t*)v.c_str();
    size_t hash = fnv1_32(data, v.length());
    return hash;
  }
};

template <typename T> struct FNV1AHash;
template <> struct FNV1AHash<std::string> {
  size_t operator()(const std::string& v) const noexcept {
    const uint8_t* data = (const uint8_t*)v.c_str();
    size_t hash = fnv1a_32(data, v.length());
    return hash;
  }
};

template <typename T> struct MurmurHash;
template <> struct MurmurHash<std::string> {
  size_t operator()(const std::string& v) const noexcept {
    const uint8_t* data = (const uint8_t*)v.c_str();
    size_t hash = murmur3(data, v.length());
    return hash;
  }
};

template <typename T> struct JenkinsHash;
template <> struct JenkinsHash<std::string> {
  size_t operator()(const std::string& v) const noexcept {
    const uint8_t* data = (const uint8_t*)v.c_str();
    size_t hash = jenkins(data, v.length());
    return hash;
  }
};

struct TestBloomFilter : ::testing::Test {
  TestBloomFilter(): bf(FNV1Hash<std::string>(), FNV1AHash<std::string>(), MurmurHash<std::string>(), JenkinsHash<std::string>()){}
  ~TestBloomFilter(){
    bf.clear();
  }

  BloomFilter<std::string, 1024, FNV1Hash<std::string>, FNV1AHash<std::string>, MurmurHash<std::string>, JenkinsHash<std::string>> bf;
};

TEST_F(TestBloomFilter, Test1){
  std::string ts1("our first test of bloom filter.");
  std::string ts2("our second test of bloom filter.");
  std::string ts3("our third test of bloom filter.");
  std::string ts4("our fourth test of bloom filter.");

  bf.add(ts1);
  bf.add(ts2);
  bf.add(ts3);

  EXPECT_TRUE(bf.test(ts1));
  EXPECT_TRUE(bf.test(ts2));
  EXPECT_TRUE(bf.test(ts3));
  EXPECT_FALSE(bf.test(ts4));
}

TEST_F(TestBloomFilter, Test2){
  std::string ts1("death");
  std::string ts2("war");
  std::string ts3("famine");
  std::string ts4("peace");

  bf.add(ts1);
  bf.add(ts2);
  bf.add(ts3);

  EXPECT_FALSE(bf.test(ts4));
}
