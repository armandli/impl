#include <gcd.h>

#include <gtest/gtest.h>

TEST(TestGCD, Test1){
  ll a = 19377;
  ll b = 721;

  ll r1 = gcd1(a, b);
  ll r2 = gcd2(a, b);

  EXPECT_EQ(r1, r2);
}

TEST(TestBGCD, Test1){
  ll a = 19377;
  ll b = 721;

  ll r1 = gcd1(a, b);
  ll r2 = bgcd1(a, b);
  ll r3 = bgcd2(a, b);
  ll r4 = bgcd3(a, b);

  EXPECT_EQ(r1, r2);
  EXPECT_EQ(r1, r3);
  EXPECT_EQ(r1, r4);
}

TEST(TestBGCD, Test2){
  ll a = 25478;
  ll b = 974;

  ll r1 = gcd1(a, b);
  ll r2 = bgcd1(a, b);
  ll r3 = bgcd2(a, b);
  ll r4 = bgcd3(a, b);

  EXPECT_EQ(r1, r2);
  EXPECT_EQ(r1, r3);
  EXPECT_EQ(r1, r4);
}

TEST(TestBGCD, Test3){
  ll a = 9999992;
  ll b = 66352;

  ll r1 = gcd1(a, b);
  ll r2 = bgcd1(a, b);
  ll r3 = bgcd2(a, b);
  ll r4 = bgcd3(a, b);

  EXPECT_EQ(r1, r2);
  EXPECT_EQ(r1, r3);
  EXPECT_EQ(r1, r4);
}

TEST(TestBGCD, Test4){
  ll a = 9999991;
  ll b = 66357;

  ll r1 = gcd1(a, b);
  ll r2 = bgcd1(a, b);
  ll r3 = bgcd2(a, b);
  ll r4 = bgcd3(a, b);

  EXPECT_EQ(r1, r2);
  EXPECT_EQ(r1, r3);
  EXPECT_EQ(r1, r4);
}

