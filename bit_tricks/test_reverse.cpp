#include <reverse.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ReverseTest, Test1){
  unsigned input = 1;
  unsigned expected = 0x80000000;
  unsigned result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test2){
  unsigned input = 0x80000000;
  unsigned expected = 1;
  unsigned result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test3){
  unsigned input = 0xAAAAAAAA;
  unsigned expected = 0x55555555;
  unsigned result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test4){
  unsigned input = 0xFFFF00;
  unsigned expected = 0xFFFF00;
  unsigned result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test5){
  unsigned input = 0xFF00FF00;
  unsigned expected = 0xFF00FF;
  unsigned result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test6){
  int input = 1;
  int expected = 0x80000000;
  int result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test7){
  int input = -2;
  int expected = 0x7FFFFFFF;
  int result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test8){
  int input = -1;
  int expected = -1;
  int result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

TEST(ReverseTest, Test9){
  unsigned long long input = 1;
  unsigned long long expected = 0x8000000000000000ULL;
  unsigned long long result = bit_reverse(input);
  EXPECT_EQ(expected, result);
}

