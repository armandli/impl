#include <gtest/gtest.h>

#include <fibonacci_hash.h>

TEST(IsPowerOfTwo, Test1){
  EXPECT_TRUE(is_power_of_two(8));
  EXPECT_TRUE(is_power_of_two(4));
  EXPECT_TRUE(is_power_of_two(2));
  EXPECT_TRUE(is_power_of_two(1));
  EXPECT_TRUE(is_power_of_two(256));
  EXPECT_TRUE(is_power_of_two(1024));
  EXPECT_TRUE(is_power_of_two(2048));
}

TEST(NumberOfBits, Test1){
  EXPECT_EQ(2, number_of_bits(4));
  EXPECT_EQ(3, number_of_bits(8));
  EXPECT_EQ(4,  number_of_bits(16));
  EXPECT_EQ(5,  number_of_bits(32));
  EXPECT_EQ(6,  number_of_bits(64));
  EXPECT_EQ(1,  number_of_bits(2));
  EXPECT_EQ(8,  number_of_bits(256));
}

TEST(BasicFibonacciHash, Test1){
  EXPECT_EQ(4, fibonacci_hash(1, 8));
  EXPECT_EQ(1, fibonacci_hash(2, 8));
  EXPECT_EQ(6, fibonacci_hash(3, 8));
  EXPECT_EQ(3, fibonacci_hash(4, 8));
  EXPECT_EQ(0, fibonacci_hash(5, 8));
  EXPECT_EQ(5, fibonacci_hash(6, 8));
  EXPECT_EQ(2, fibonacci_hash(7, 8));
  EXPECT_EQ(7, fibonacci_hash(8, 8));
  EXPECT_EQ(4, fibonacci_hash(9, 8));
  EXPECT_EQ(1, fibonacci_hash(10, 8));
  EXPECT_EQ(6, fibonacci_hash(11, 8));
  EXPECT_EQ(3, fibonacci_hash(12, 8));
  EXPECT_EQ(0, fibonacci_hash(13, 8));
}

TEST(FibonacciHash, Test1){
  EXPECT_EQ(3, fibonacci_hash_opt(4, 8));
  EXPECT_EQ(7, fibonacci_hash_opt(8, 8));
  EXPECT_EQ(3, fibonacci_hash_opt(12, 8));
  EXPECT_EQ(7, fibonacci_hash_opt(16, 8));
  EXPECT_EQ(2, fibonacci_hash_opt(20, 8));
  EXPECT_EQ(6, fibonacci_hash_opt(24, 8));
}
