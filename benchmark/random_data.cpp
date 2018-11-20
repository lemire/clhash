#include "boost/container_hash/hash.hpp"
#include "clhash.h"
#include "hybrid_hash.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <vector>

// farmhash header
#define XXH_INLINE_ALL
#include "xxhash.h"

namespace {
    class RandomVector {
      public:
        RandomVector() : rgn(std::chrono::system_clock::now().time_since_epoch().count()) {}

        RandomVector(unsigned seed) : rgn(seed) {}

        template <typename T> std::vector<T> generate(const size_t len) {
            std::vector<T> data(len);
            for (size_t idx = 0; idx < len; ++idx) { data[idx] = rgn(); }
            return data;
        }

      private:
        std::mt19937 rgn;
    };

    template <typename T> std::vector<T> generate_random_data() {
        const char *var = std::getenv("LEN");
        if (var == nullptr) {
            throw std::runtime_error("LEN environment variable is required!");
        }
        int len = std::stoi(var);
        std::cout << "Vector size: " << len << "\n";
        RandomVector gen;
        return gen.generate<T>(len);
    }

} // namespace

const auto idata = generate_random_data<int>();
const auto ddata = generate_random_data<double>();

/**
 * Benchmark for int data
 */
// hash functions
// void std_hash_int(benchmark::State &state) {
//     std::hash<int> h;
//     for (auto _ : state) { benchmark::DoNotOptimize(
//             uint64_t code = 0;
//             for (size_t idx = 0; idx < idata.size(); ++idx)
//             code += h(idata[idx]));
//     }
// }
// // Register the function as a benchmark
// BENCHMARK(std_hash_int);

void clhash_int(benchmark::State &state) {
    util::CLHash clhash;
    for (auto _ : state) { benchmark::DoNotOptimize(clhash(idata.data(), idata.size())); }
}
BENCHMARK(clhash_int);


void boost_hash_int(benchmark::State &state) {
    boost::hash<std::vector<int>> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(idata)); }
}
BENCHMARK(boost_hash_int);

// xxHash
void xxhash_int(benchmark::State &state) {
    unsigned long long const seed = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(XXH64((char*)(idata.data()), idata.size() * sizeof(int), seed));
    }
}
BENCHMARK(xxhash_int);

// FarmHash
void farmhash_int(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(util::Hash((char*)idata.data(), idata.size() * sizeof(int)));
    }
}
BENCHMARK(farmhash_int);

void hybrid_hash_int(benchmark::State &state) {
    util::HybridHash h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(idata.data(), idata.size())); }
}
BENCHMARK(hybrid_hash_int);

/**
 * Benchmark for double data
 */
void clhash_double(benchmark::State &state) {
    util::CLHash clhash;
    for (auto _ : state) { benchmark::DoNotOptimize(clhash(ddata.data(), ddata.size())); }
}
// Register the function as a benchmark
BENCHMARK(clhash_double);

void boost_hash_double(benchmark::State &state) {
    boost::hash<std::vector<double>> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(ddata)); }
}
// Register the function as a benchmark
BENCHMARK(boost_hash_double);

// xxHash
void xxhash_double(benchmark::State &state) {
    unsigned long long const seed = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(XXH64((char*)(ddata.data()), ddata.size() * sizeof(double), seed));
    }
}
BENCHMARK(xxhash_double);

// FarmHash
void farmhash_double(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(util::Hash((char*)ddata.data(), ddata.size() * sizeof(double)));
    }
}
BENCHMARK(farmhash_double);

void hybrid_hash_double(benchmark::State &state) {
    util::HybridHash h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(ddata.data(), ddata.size())); }
}
BENCHMARK(hybrid_hash_double);

BENCHMARK_MAIN();
