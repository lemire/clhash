#include "clhash.h"
#include <cassert>
#include <cstdio>
#include <unordered_map>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

const std::string long_string = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness. Prudence, indeed, will dictate that Governments long established should not be changed for light and transient causes; and accordingly all experience hath shewn, that mankind are more disposed to suffer, while evils are sufferable, than to right themselves by abolishing the forms to which they are accustomed. But when a long train of abuses and usurpations, pursuing invariably the same Object evinces a design to reduce them under absolute Despotism, it is their right, it is their duty, to throw off such Government, and to provide new Guards for their future security.";

template <typename T> void test() {
    clhash::CLHash h1, h2, h3(5, 27);
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
    clhash::CLHash h;
    CHECK(h(7723291ULL) == 6077153076896802708ULL);
    CHECK(h(1ULL) == 6105994745979306188ULL);
    CHECK(h(29ULL) == 6105994779349333020ULL);
}

TEST_CASE("Basic string tests", "") {
    clhash::CLHash h1(1, 4);
    std::hash<std::string> h2;
    std::string short_string("Hello world!");

    // Use std::hash for short strings
    CHECK(h1(short_string) == h2(std::string(short_string)));
    
    // Use CLHash for long strings
    CHECK(h1(long_string) != h2(long_string));

    // Different string should have different hash values.
    CHECK(h1(short_string.data(), 2) != h1(short_string.data(), 3));
}

// TODO: Need to update this test.
TEST_CASE("std::vector") {
    clhash::CLHash hash_fcn;
    std::vector<int> int_data1{1, 2, 3, 4, 5};
    std::vector<int> int_data2{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28};
    CHECK(hash_fcn(int_data1.data(), int_data1.size()) ==
          3190504406307648843UL); // Use std::hash
    CHECK(hash_fcn(int_data2.data(), int_data2.size()) ==
          14240744024781019794UL); // Use clhash::CLHash
    CHECK(hash_fcn(int_data2.data(), int_data2.size()) ==
          hash_fcn(int_data2)); // Use clhash::CLHash
}

TEST_CASE("std::unordered_map") {
    auto dict = std::unordered_map<std::string, int, clhash::CLHash>();
    dict.emplace("Hello", 1);
    dict.emplace(long_string, 2);
    dict.emplace("Hello", 1);
    dict.emplace(long_string, 2);
    CHECK(dict.size() == 2); // There should be two elements only.

    for (auto item : dict) { std::cout << item.first << " - " << item.second << "\n"; }

    // Check that we can insert data properly.
    CHECK(dict["Hello"] == 1);
    CHECK(dict[long_string] == 2);
}
