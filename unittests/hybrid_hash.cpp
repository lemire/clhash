#include "hybrid_hash.h"
#include <cassert>
#include <cstdio>
#include <unordered_map>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

template <typename T> void test() {
    util::HybridHash h1, h2, h3(5, 27);
    T short_array[6] = {1, 3, 9, 22, 47, 24343};
    T long_array[30] = {1, 3, 9, 22, 47, 24343, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                        1, 3, 9, 22, 47, 24343, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    std::vector<T> short_vector(std::begin(short_array), std::end(short_array));
    std::vector<T> long_vector(std::begin(long_array), std::end(long_array));

    CHECK(h1(short_array, 6) ==
          h2(short_vector)); // Same seeds must produce the same hash values.
    CHECK(h1(short_array) !=
          h3(short_vector)); // Different seeds must produce different hash values.

    // // Check hash values of scalars
    CHECK(h1(short_vector[5]) == h2(short_array[5]));
    CHECK(h1(short_vector[5]) == h3(short_array[5])); // We use farmhash for short vector.

    // Make sure that we compute the hash using the value of a variable.
    T value1 = 1.0, value2 = 2.0;
    CHECK(h1(value1) != h2(value2));
    CHECK(h1(value1) == h2(&value1, 1));

    value2 = value1;
    CHECK(h1(value1) == h2(value2));
    CHECK(h1(value1) == h2(&value2, 1));

    // We use clhash for big enough buffer
    CHECK(h1(long_vector) == h2(long_vector));
    CHECK(h1(long_vector) != h1(short_vector));
    CHECK(h1(long_vector) != h3(long_vector)); // clhash return different hash code for different seeds.
    
}

TEST_CASE("Regular data type tests", "") {
    test<int>();
    test<float>();
    test<double>();
    test<size_t>();
}
