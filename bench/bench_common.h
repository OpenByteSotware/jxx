#pragma once
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

struct BenchTimer {
    using clock = std::chrono::steady_clock;
    clock::time_point t0{};
    void start() { t0 = clock::now(); }
    double stop_seconds() const {
        auto t1 = clock::now();
        return std::chrono::duration<double>(t1 - t0).count();
    }
};

inline std::vector<std::string> make_random_keys(std::size_t n, std::size_t keyLen = 16, uint64_t seed = 12345) {
    static const char alnum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> dist(0, sizeof(alnum) - 2);
    std::vector<std::string> keys;
    keys.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::string s;
        s.resize(keyLen);
        for (std::size_t j = 0; j < keyLen; ++j) s[j] = alnum[dist(rng)];
        keys.emplace_back(std::move(s));
    }
    return keys;
}

inline std::vector<int> make_values(std::size_t n, uint64_t seed = 67890) {
    std::mt19937 rng(static_cast<unsigned>(seed));
    std::uniform_int_distribution<int> dist(0, 1'000'000);
    std::vector<int> v(n);
    for (std::size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

inline void print_result(const std::string& name, double secs, std::size_t ops) {
    const double ops_sec = ops / secs;
    std::cout << name << ": " << ops << " ops in " << secs << " s  ("
              << static_cast<long long>(ops_sec) << " ops/s)\n";
}

// Simple spin barrier for C++17
class SpinBarrier {
public:
    explicit SpinBarrier(int count) : target_(count), count_(0), go_(false) {}
    void arrive() {
        int old = count_.fetch_add(1, std::memory_order_acq_rel) + 1;
        if (old == target_) {
            go_.store(true, std::memory_order_release);
        } else {
            while (!go_.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
        }
    }
private:
    const int target_;
    std::atomic<int> count_;
    std::atomic<bool> go_;
};