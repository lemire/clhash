#include "clhash.h"
#include <cassert>
#include <cstdio>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

template <typename T> void test() {
    lemire::clhasher h1, h2, h3(5, 27);
    T array[6] = {1, 3, 9, 22, 47, 24343};
    std::vector<T> vector = {1, 3, 9, 22, 47, 24343};
    CHECK(h1(array) == h2(vector)); // Same seeds must produce the same hash values.
    CHECK(h1(array) != h3(vector)); // Different seeds must produce different hash values.

    // Check hash values of scalars
    CHECK(h1(vector[5]) == h2(array[5]));
    CHECK(h1(vector[5]) != h3(array[5]));
}

TEST_CASE("Regular data type tests", "") {
    test<int>();
    test<float>();
    test<double>();
    test<size_t>();
    lemire::clhasher h;
    CHECK(h(7723291ULL) == 6077153076896802708ULL);
    CHECK(h(1ULL) == 6105994745979306188ULL);
    CHECK(h(29ULL) == 6105994779349333020ULL);
}


TEST_CASE("Basic string tests", "") {
    lemire::clhasher h(1, 4);
    const char *str = "Hello world!";
    CHECK(h(str) == h(std::string(str)));
}
