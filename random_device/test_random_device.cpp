#include <pcg.h>
#include <xorshift.h>
#include <splitmix.h>
#include <ctime>
#include <unistd.h>

#include <vector>
#include <iostream>

#include <gtest/gtest.h>

namespace s = std;

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

TEST(XORSHIFT, TestDistribution1){
  xorshift dev(time(nullptr));

  size_t sz = 100;
  s::vector<int> dist(sz);
  for (size_t i = 0; i < 100000; ++i){
    int val = dev() % sz;
    dist[val]++;
  }

  s::cout << "XORSHIFT Distribution:" << '\n';
  for (size_t i = 0; i < sz; ++i){
    for (int j = 0; j < dist[i]; j += 100)
      s::cout << '*';
    s::cout << '\n';
  }
  s::cout << s::endl;
}

TEST(SPLITMIX, TestDistribution1){
  splitmix dev(time(nullptr));

  size_t sz = 100;
  s::vector<int> dist(sz);
  for (size_t i = 0; i < 100000; ++i){
    int val = dev() % sz;
    dist[val]++;
  }

  s::cout << "SPLITMIX Distribution:" << '\n';
  for (size_t i = 0; i < sz; ++i){
    for (int j = 0; j < dist[i]; j += 100)
      s::cout << '*';
    s::cout << '\n';
  }
  s::cout << s::endl;
}

TEST(PCG, TestDistribution1){
  pcg dev(time(nullptr));

  size_t sz = 100;
  s::vector<int> dist(sz);
  for (size_t i = 0; i < 100000; ++i){
    int val = dev() % sz;
    dist[val]++;
  }

  s::cout << "PCG Distribution:" << '\n';
  for (size_t i = 0; i < sz; ++i){
    for (int j = 0; j < dist[i]; j += 100)
      s::cout << '*';
    s::cout << '\n';
  }
  s::cout << s::endl;
}
