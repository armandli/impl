#include <cstdint>

#include <pcg.h>
#include <xorshift.h>
#include <splitmix.h>

#include <vector>
#include <random>

#include <benchmark/benchmark.h>

namespace s = std;
namespace b = benchmark;

static void bm_pcg(b::State& st){
  pcg gen;
  s::vector<uint32_t> res;
  for (auto _ : st){
    res.push_back(gen());
  }
}
BENCHMARK(bm_pcg);

static void bm_splitmix(b::State& st){
  splitmix gen;
  s::vector<uint32_t> res;
  for (auto _ : st){
    res.push_back(gen());
  }
}
BENCHMARK(bm_splitmix);

static void bm_xorshift(b::State& st){
  xorshift gen;
  s::vector<uint32_t> res;
  for (auto _ : st){
    res.push_back(gen());
  }
}
BENCHMARK(bm_xorshift);

static void bm_rand(b::State& st){
  s::vector<uint32_t> res;
  for (auto _ : st){
    res.push_back(rand());
  }
}
BENCHMARK(bm_rand);

static void bm_mt19937(b::State& st){
  s::vector<uint32_t> res;
  s::mt19937 mt;
  for (auto _ : st){
    res.push_back(mt());
  }
}
BENCHMARK(bm_mt19937);

static void bm_mt19937_64(b::State& st){
  s::vector<uint32_t> res;
  s::mt19937_64 mt;
  for (auto _ : st){
    res.push_back(mt());
  }
}
BENCHMARK(bm_mt19937_64);
