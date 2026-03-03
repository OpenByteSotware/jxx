#include <iostream>
#include <thread>
#include <vector>
#include <optional>
#include "jxx.h"
#include "bench_common.h"

using jxx::util::Hashtable;

// Worker that writes & reads disjoint key ranges (low contention).
static void worker_low_contention(Hashtable<std::string,int>& ht,
                                  const std::vector<std::string>& keys,
                                  const std::vector<int>& vals,
                                  std::size_t begin, std::size_t end,
                                  SpinBarrier& barrier)
{
    barrier.arrive();
    for (std::size_t i = begin; i < end; ++i) {
        ht.put(keys[i], std::optional<int>{vals[i]});
        auto v = ht.get(keys[i]);
        if (!v || *v != vals[i]) { /* correctness guard; ignore */ }
    }
}

// Worker that writes/read from a small hot set (high contention).
static void worker_high_contention(Hashtable<std::string,int>& ht,
                                   const std::vector<std::string>& hotKeys,
                                   int iters_per_thread,
                                   SpinBarrier& barrier)
{
    barrier.arrive();
    const int m = static_cast<int>(hotKeys.size());
    for (int i = 0; i < iters_per_thread; ++i) {
        const std::string& k = hotKeys[i % m];
        ht.put(k, std::optional<int>{i});
        (void)ht.get(k);
    }
}

int main() {
    const std::size_t N = 300'000;
    const int threads = std::max(2u, std::thread::hardware_concurrency());
    auto keys = make_random_keys(N);
    auto vals = make_values(N);

    {
        std::cout << "=== Hashtable MT: Low contention (disjoint keys) ===\n";
        Hashtable<std::string,int> ht;
        ht.ensureCapacity(N);

        SpinBarrier barrier(threads);
        BenchTimer t;
        std::vector<std::thread> pool;
        pool.reserve(threads);

        const std::size_t per = N / threads;
        t.start();
        for (int tid = 0; tid < threads; ++tid) {
            std::size_t b = tid * per;
            std::size_t e = (tid == threads - 1) ? N : (b + per);
            pool.emplace_back(worker_low_contention, std::ref(ht), std::cref(keys), std::cref(vals), b, e, std::ref(barrier));
        }
        for (auto& th : pool) th.join();
        double secs = t.stop_seconds();
        print_result("put+get (total)", secs, N * 2);
    }

    {
        std::cout << "\n=== Hashtable MT: High contention (hot set) ===\n";
        Hashtable<std::string,int> ht;
        const int hot = 1024; // small hot keyset
        auto hotKeys = make_random_keys(hot, 16, 777);
        const int iters_per_thread = 200'000; // total ops per thread (put+get pair counted as 2)
        const std::size_t total_ops = static_cast<std::size_t>(threads) * iters_per_thread * 2;

        SpinBarrier barrier(threads);
        BenchTimer t;
        std::vector<std::thread> pool;
        pool.reserve(threads);

        t.start();
        for (int tid = 0; tid < threads; ++tid) {
            pool.emplace_back(worker_high_contention, std::ref(ht), std::cref(hotKeys), iters_per_thread, std::ref(barrier));
        }
        for (auto& th : pool) th.join();
        double secs = t.stop_seconds();
        print_result("put+get (total)", secs, total_ops);
    }

    return 0;
}
