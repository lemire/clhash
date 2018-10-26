#include "clhash.h"
#include <benchmark/benchmark.h>
#include <functional>

const std::string test_string("This is my test string");

// hash functions
void std_hash_string(benchmark::State &state) {
    std::hash<std::string> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(test_string)); }
}
// Register the function as a benchmark
BENCHMARK(std_hash_string);

void clhash_string(benchmark::State &state) {
    lemire::clhasher clhash;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clhash(test_string.data(), test_string.size()));
    }
}
// Register the function as a benchmark
BENCHMARK(clhash_string);

BENCHMARK_MAIN();
