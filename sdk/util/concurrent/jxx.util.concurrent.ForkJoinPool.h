#pragma once

#include <algorithm>
#include <cstddef>
#include <future>
#include <thread>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/jxx.util.concurrent.AbstractExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"

namespace jxx::util::concurrent {

class ForkJoinPool final : public AbstractExecutorService {
public:
    using JxxSuper = AbstractExecutorService;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<
        ForkJoinPool, JxxSuper, ExecutorService>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    explicit ForkJoinPool(::jxx::lang::jint parallelism);
    ~ForkJoinPool() override = default;

    static ::jxx::Ptr<ForkJoinPool> commonPool();
    ::jxx::lang::jint getParallelism() const noexcept;

    void execute(const ::jxx::Ptr<::jxx::lang::Runnable>& command) override;
    void shutdown() override;
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> shutdownNow() override;
    ::jxx::lang::jbool isShutdown() override;
    ::jxx::lang::jbool isTerminated() override;
    ::jxx::lang::jbool awaitTermination(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override;
    ::jxx::Ptr<wildcard::FutureAny> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task) override;

    template <typename F>
    void parallelFor(std::size_t size, ::jxx::lang::jlong threshold, F function) const {
        if (size == 0) return;
        if (threshold <= 0 || size < static_cast<std::size_t>(threshold) ||
            parallelism_ <= 1) {
            function(0, size);
            return;
        }
        const auto tasks = std::min<std::size_t>(
            size, static_cast<std::size_t>(parallelism_));
        const auto chunk = (size + tasks - 1) / tasks;
        std::vector<std::future<void>> futures;
        for (std::size_t begin = 0; begin < size; begin += chunk) {
            const auto end = std::min(size, begin + chunk);
            futures.emplace_back(std::async(
                std::launch::async, [=, &function] { function(begin, end); }));
        }
        for (auto& future : futures) future.get();
    }

    template <typename R, typename F, typename Reduce>
    R parallelReduce(std::size_t size, ::jxx::lang::jlong threshold,
                     R identity, F function, Reduce reducer) const {
        if (size == 0) return identity;
        if (threshold <= 0 || size < static_cast<std::size_t>(threshold) ||
            parallelism_ <= 1) return function(0, size);
        const auto tasks = std::min<std::size_t>(
            size, static_cast<std::size_t>(parallelism_));
        const auto chunk = (size + tasks - 1) / tasks;
        std::vector<std::future<R>> futures;
        for (std::size_t begin = 0; begin < size; begin += chunk) {
            const auto end = std::min(size, begin + chunk);
            futures.emplace_back(std::async(
                std::launch::async, [=, &function] { return function(begin, end); }));
        }
        auto result = identity;
        for (auto& future : futures) result = reducer(result, future.get());
        return result;
    }

private:
    ::jxx::lang::jint parallelism_;
    ::jxx::lang::jbool common_;
    ::jxx::Ptr<ThreadPoolExecutor> executor_;
};

} // namespace jxx::util::concurrent
