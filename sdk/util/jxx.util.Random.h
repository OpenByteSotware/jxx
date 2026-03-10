
#pragma once
#include <random>
#include "lang/jxx.lang.h"

namespace jxx { namespace util {

class Random {
public:
    Random() : rng_(std::random_device{}()) {}
    explicit Random(long long seed) : rng_(static_cast<std::mt19937_64::result_type>(seed)) {}

    int nextInt() { return static_cast<int>(dist32_(rng_)); }
    int nextInt(int bound) { std::uniform_int_distribution<int> d(0, bound-1); return d(rng_); }
    long long nextLong() { return static_cast<long long>(dist64_(rng_)); }
    bool nextBoolean() { std::bernoulli_distribution d(0.5); return d(rng_); }
    double nextDouble() { std::uniform_real_distribution<double> d(0.0, 1.0); return d(rng_); }

    void nextBytes(jxx::lang::ByteArray& b) {
        for (size_t i=0;i<b.size();++i) b[i] = static_cast<std::uint8_t>(dist32_(rng_) & 0xFF);
    }

private:
    std::mt19937_64 rng_;
    std::uniform_int_distribution<std::uint32_t> dist32_;
    std::uniform_int_distribution<std::uint64_t> dist64_;
};

}} // namespace jxx::util
