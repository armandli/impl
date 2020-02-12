#include <distribution.h>

#include <random>
#include <iostream>

#include <gtest/gtest.h>

namespace s = std;

s::default_random_engine& engine(){
  static s::default_random_engine eng;
  return eng;
}

TEST(TestBeta, TestBeta1){
  beta_distribution<float> dist;
  float x = dist(engine());

  s::cout << x << s::endl;
}

TEST(TestBeta, TestBeta2){
  beta_distribution<float> dist(0.1, 1.0);
  float x = dist(engine());

  s::cout << x << s::endl;
}

TEST(TestBeta, TestBeta3){
  beta_distribution<float> dist(1.0, 0.1);
  float x = dist(engine());

  s::cout << x << s::endl;
}
