/**
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

#include "detail/clhash_impl.h"
#include <string>
#include <vector>

namespace clhash {
    class CLHash {
      public:
        CLHash(uint64_t seed1 = SEED1, uint64_t seed2 = SEED2)
            : random_data_(get_random_key_for_clhash(seed1, seed2)) {}

        ~CLHash() { std::free((void *)random_data_); }

        template <typename T> uint64_t operator()(const T *data, const size_t len) const {
            return clhash(random_data_, reinterpret_cast<const char *>(data), len * sizeof(T));
        }

        // Use std::hash to compute the hash value of string with size
        // less then 192 bytes. This decision is based on our benchmark
        // results.
        uint64_t operator()(const std::string &str) const {
            return (str.size() < CUT_OFF_LENGTH) ? string_hash_fcn(str) :
                                                operator()(str.data(), str.size());
        }

        // For other data types
        template <typename T> uint64_t operator()(const T &input) const {
            return operator()(&input, sizeof(T));
        }

        template <typename T> uint64_t operator()(const std::vector<T> &input) const {
            return operator()(input.data(), sizeof(T) * input.size());
        }

      private:
        const void *random_data_;
        std::hash<std::string> string_hash_fcn;
        static constexpr uint64_t SEED1 = 137;
        static constexpr uint64_t SEED2 = 777;
        static constexpr size_t CUT_OFF_LENGTH = 192;
    };
} // namespace clhash
