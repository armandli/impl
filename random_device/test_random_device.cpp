#include <pcg.h>
#include <xorshift.h>
#include <splitmix.h>
#include <ctime>
#include <unistd.h>

#include <gtest/gtest.h>

TEST(PCG, Test1){
  pcg dev;
  int t1 = dev(), t2 = dev();
  EXPECT_TRUE(t1 != t2);
}

TEST(XORSHIFT, Test1){
  xorshift dev;
  int t1 = dev(), t2 = dev();
  EXPECT_TRUE(t1 != t2);
}

TEST(SPLITMIX, Test1){
  splitmix dev;
  int t1 = dev(), t2 = dev();
  EXPECT_TRUE(t1 != t2);
}

TEST(PCG, TestEqual){
  pcg dev1, dev2;
  int t1 = dev1(), t2 = dev2();
  EXPECT_EQ(t1, t2);
}

TEST(XORSHIFT, TestEqual){
  xorshift dev1, dev2;
  int t1 = dev1(), t2 = dev2();
  EXPECT_EQ(t1, t2);
}

TEST(SPLITMIX, TestEqual){
  splitmix dev1, dev2;
  int t1 = dev1(), t2 = dev2();
  EXPECT_EQ(t1, t2);
}

TEST(XORSHIFT, TestNotEqual){
  xorshift dev1(time(nullptr));
  sleep(1);
  xorshift dev2(time(nullptr));
  int t1 = dev1(), t2 = dev2();
  EXPECT_FALSE(t1 == t2);
}

TEST(SPLITMIX, TestNotEqual){
  splitmix dev1(time(nullptr));
  sleep(1);
  splitmix dev2(time(nullptr));
  int t1 = dev1(), t2 = dev2();
  EXPECT_FALSE(t1 == t2);
}

TEST(PCG, TestNotEqual){
  pcg dev1(time(nullptr));
  sleep(1);
  pcg dev2(time(nullptr));
  int t1 = dev1(), t2 = dev2();
  EXPECT_FALSE(t1 == t2);
}
