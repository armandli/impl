#include <jenkins.h>

#include <string>
#include <gtest/gtest.h>

template <typename T> class JenkinsHash;

template <>
class JenkinsHash<std::string> {
public:
  size_t operator()(const std::string& v) const {
    const uint8_t* data = (const uint8_t*)v.c_str();
    size_t hv = jenkins(data, v.length());
    return hv;
  }
};

template <>
class JenkinsHash<int> {
public:
  size_t operator()(const int& v) const {
    const uint8_t* data = (const uint8_t*)&v;
    size_t hv = jenkins(data, sizeof(int));
    return hv;
  }
};

TEST(HashString, Test1){
  std::string t = "hello world";
  JenkinsHash<std::string> hash;
  EXPECT_EQ(0x3e4a5a57, hash(t));
}

TEST(HashInt, Test1){
  int t = 1337;
  JenkinsHash<int> hash;
  EXPECT_EQ(0x9300741f, hash(t));
}
