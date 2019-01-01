#include <naive_trie.h>

#include <string>
#include <benchmark/benchmark.h>

namespace s = std;
namespace b = benchmark;

const s::string ins = "jifieufpeuifbaiuwfb93uhfrnp3iunfofqi320rij3pnokneoj3irb3ybou3yro83uodwekdjo39rj032ijfnpe9i3pfn4unfjenfwsdijjkdnslkdp092ep1982u3hjn;dkldmeojf";

static void BM_insertion(b::State& st){
  for(auto _ : st){
    NaiveTrie trie;
    trie.insert(ins);
  }
}
BENCHMARK(BM_insertion);
