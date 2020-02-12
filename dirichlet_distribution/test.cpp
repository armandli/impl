#include <distribution.h>

#include <random>
#include <iostream>

#include <gtest/gtest.h>

namespace s = std;

s::default_random_engine& engine(){
  static s::default_random_engine eng;
  return eng;
}

TEST(TestDirichlet, TestDirichlet1){
  constexpr size_t SZ = 10;
  dirichlet_distribution<SZ, float> dist;
  s::array<float, SZ> xs = dist(engine());

  for (float v : xs)
    s::cout << v << " ";
  s::cout << s::endl;

  float sum = s::accumulate(s::begin(xs), s::end(xs), 0.F);
  EXPECT_FLOAT_EQ(1.0, sum);
}

TEST(TestDirichlet, TestDirichlet2){
  constexpr size_t SZ = 8;
  dirichlet_distribution<SZ, float> dist({0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1});
  s::array<float, SZ> xs = dist(engine());

  for (float v : xs)
    s::cout << v << " ";
  s::cout << s::endl;

  float sum = s::accumulate(s::begin(xs), s::end(xs), 0.F);
  EXPECT_FLOAT_EQ(1.0, sum);
}

TEST(TestDirichlet, TestDirichlet3){
  constexpr size_t SZ = 8;
  dirichlet_distribution<SZ, float> dist({10., 10., 10., 10., 10., 10., 10., 10.});
  s::array<float, SZ> xs = dist(engine());

  for (float v : xs)
    s::cout << v << " ";
  s::cout << s::endl;

  float sum = s::accumulate(s::begin(xs), s::end(xs), 0.F);
  EXPECT_FLOAT_EQ(1.0, sum);
}

TEST(TestDirichlet, TestDirichlet4){
  constexpr size_t SZ = 8;
  dirichlet_distribution<SZ, float> dist({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8});
  s::array<float, SZ> xs = dist(engine());

  for (float v : xs)
    s::cout << v << " ";
  s::cout << s::endl;

  float sum = s::accumulate(s::begin(xs), s::end(xs), 0.F);
  EXPECT_FLOAT_EQ(1.0, sum);
}
