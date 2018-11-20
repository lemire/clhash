#pragma once

#include "detail/clhash_impl.h"
#include <string>
#include <vector>

// farmhash header
#include "farmhash.cc"
#include "farmhash.h"

namespace util {
    // This is the combination of the clhash and farmhash.
    class HybridHash {
      public:
        HybridHash(uint64_t seed1 = SEED1, uint64_t seed2 = SEED2)
            : random_data_(get_random_key_for_clhash(seed1, seed2)) {}

        ~HybridHash() { std::free((void *)random_data_); }

        uint64_t operator()(const char *data, const size_t len) {
            return (len <= CUT_OFF_LENGTH) ? util::Hash(data, len)
                                           : clhash(random_data_, data, len);
        }

        uint64_t operator()(const std::string &s) { return operator()(s.data(), s.size()); }

        template <typename T> uint64_t operator()(const T *data, const size_t len) {
            return operator()((const char *)data, len * sizeof(T));
        }

        template <typename T> uint64_t operator()(const std::vector<T> &data) {
            return operator()(data.data(), data.size());
        }

        template <typename T> uint64_t operator()(const T value) {
            return operator()(&value, 1);
        }

      private:
        const void *random_data_;
        static constexpr uint64_t SEED1 = 137;
        static constexpr uint64_t SEED2 = 777;

        // The cut-off threshold might not be consistence for different CPU architectures,
        // however, 96 bytes seems to be a good number.
        static constexpr size_t CUT_OFF_LENGTH = 96;
    };
} // namespace util
