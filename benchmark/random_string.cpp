#include "boost/container_hash/hash.hpp"
#include "clhash.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>

class CharGenerator {
  public:
    std::string operator()(const size_t len) {
        std::string str(len, 0);
        for (auto idx = 0; idx < len; ++idx) { str[idx] = valid_characters[rgn() % N]; }
        std::cout << "Generated random string: " << str << "\n";
        return str;
    }

  private:
    std::mt19937 rgn;
    static constexpr int N = 62;
    const std::array<char, N> valid_characters = {
        {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
         'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
         'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
         'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}};
};

std::string generate_random_string() {
    const char *var = std::getenv("LEN");
    if (var == nullptr) { throw std::runtime_error("LEN environment variable is required!"); }
    int len = std::stoi(var);
    std::cout << "String length: " << len << "\n";
    CharGenerator gen;
    return gen(len);
}

const std::string test_string = generate_random_string();

// hash functions
void std_hash_string(benchmark::State &state) {
    std::hash<std::string> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(test_string)); }
}
// Register the function as a benchmark
BENCHMARK(std_hash_string);

void clhash_string(benchmark::State &state) {
    lemire::clhash::CLHash clhash;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clhash(test_string.data(), test_string.size()));
    }
}
// Register the function as a benchmark
BENCHMARK(clhash_string);

void boost_hash_string(benchmark::State &state) {
    boost::hash<std::string> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(test_string)); }
}
// Register the function as a benchmark
BENCHMARK(boost_hash_string);

BENCHMARK_MAIN();
