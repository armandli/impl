#include <gcd.h>

#include <chrono>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

namespace s = std;

static void BM_gcd1(benchmark::State& state){
  s::vector<ll> input1 = {8734387, 38636, 28397, 848786467};
  s::vector<ll> input2 = {386,     347837,384387,6376172};
  s::vector<ll> res;
  for (auto _ : state){
    for (size_t i = 0; i < input1.size(); ++i){
      res.push_back(gcd1(input1[i], input2[i]));
    }
  }
}
BENCHMARK(BM_gcd1);

static void BM_gcd2(benchmark::State& state){
  s::vector<ll> input1 = {8734387, 38636, 28397, 848786467};
  s::vector<ll> input2 = {386,     347837,384387,6376172};
  s::vector<ll> res;
  for (auto _ : state){
    for (size_t i = 0; i < input1.size(); ++i){
      res.push_back(gcd2(input1[i], input2[i]));
    }
  }
}
BENCHMARK(BM_gcd2);

static void BM_bgcd1(benchmark::State& state){
  s::vector<ll> input1 = {8734387, 38636, 28397, 848786467};
  s::vector<ll> input2 = {386,     347837,384387,6376172};
  s::vector<ll> res;
  for (auto _ : state){
    for (size_t i = 0; i < input1.size(); ++i){
      res.push_back(bgcd1(input1[i], input2[i]));
    }
  }
}
BENCHMARK(BM_bgcd1);

static void BM_bgcd2(benchmark::State& state){
  s::vector<ll> input1 = {8734387, 38636, 28397, 848786467};
  s::vector<ll> input2 = {386,     347837,384387,6376172};
  s::vector<ll> res;
  for (auto _ : state){
    for (size_t i = 0; i < input1.size(); ++i){
      res.push_back(bgcd2(input1[i], input2[i]));
    }
  }
}
BENCHMARK(BM_bgcd2);

static void BM_bgcd3(benchmark::State& state){
  s::vector<ll> input1 = {8734387, 38636, 28397, 848786467};
  s::vector<ll> input2 = {386,     347837,384387,6376172};
  s::vector<ll> res;
  for (auto _ : state){
    for (size_t i = 0; i < input1.size(); ++i){
      res.push_back(bgcd3(input1[i], input2[i]));
    }
  }
}
BENCHMARK(BM_bgcd3);

BENCHMARK_MAIN();
