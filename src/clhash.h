/*
 * CLHash is a very fast hashing function that uses the
 * carry-less multiplication and SSE instructions.
 *
 * Daniel Lemire, Owen Kaser, Faster 64-bit universal hashing
 * using carry-less multiplications, Journal of Cryptographic Engineering (to appear)
 *
 * Best used on recent x64 processors (Haswell or better).
 *
 * Compile option: if you define BITMIX during compilation, extra work is done to
 * pass smhasher's avalanche test succesfully. Disabled by default.
 **/

#pragma once

#include <string>
#include <vector>
#include "detail/clhash_impl.h"

namespace lemire {
    struct clhasher {
        const void *random_data_;
        clhasher(uint64_t seed1 = 137, uint64_t seed2 = 777)
            : random_data_(get_random_key_for_clhash(seed1, seed2)) {}
        template <typename T> uint64_t operator()(const T *data, const size_t len) const {
            return clhash(random_data_, (const char *)data, len * sizeof(T));
        }
        uint64_t operator()(const char *str) const { return operator()(str, std::strlen(str)); }
        template <typename T> uint64_t operator()(const T &input) const {
            return operator()((const char *)&input, sizeof(T));
        }
        template <typename T> uint64_t operator()(const std::vector<T> &input) const {
            return operator()((const char *)input.data(), sizeof(T) * input.size());
        }
        uint64_t operator()(const std::string &str) const {
            return operator()(str.data(), str.size());
        }
        ~clhasher() { std::free((void *)random_data_); }
    };
} // namespace lemire
