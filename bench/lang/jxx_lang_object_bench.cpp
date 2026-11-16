#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.Object.h"

namespace {

constexpr int kBenchmarkRepetitions = 15;

double percentile(const std::vector<double>& samples, double rank) {
    if (samples.empty()) {
        return 0.0;
    }

    auto sorted = samples;
    std::sort(sorted.begin(), sorted.end());
    if (sorted.size() == 1U) {
        return sorted.front();
    }

    const double position = rank * static_cast<double>(sorted.size() - 1U);
    const auto lower = static_cast<std::size_t>(position);
    const auto upper = std::min(lower + 1U, sorted.size() - 1U);
    const double fraction = position - static_cast<double>(lower);
    return sorted[lower] + ((sorted[upper] - sorted[lower]) * fraction);
}

double p90(const std::vector<double>& samples) {
    return percentile(samples, 0.90);
}

double p95(const std::vector<double>& samples) {
    return percentile(samples, 0.95);
}

double p99(const std::vector<double>& samples) {
    return percentile(samples, 0.99);
}

void initializeJxxOnce() {
    static const bool initialized = [] {
        ::jxx::lang::initializeClasses();
        return true;
    }();
    benchmark::DoNotOptimize(initialized);
}

void BM_Object_JxxNew(benchmark::State& state) {
    initializeJxxOnce();
    for (auto _ : state) {
        auto object = ::jxx::NEW<::jxx::lang::Object>();
        benchmark::DoNotOptimize(object);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_Object_StdMakeSharedBaseline(benchmark::State& state) {
    initializeJxxOnce();
    for (auto _ : state) {
        auto object = std::make_shared<::jxx::lang::Object>();
        benchmark::DoNotOptimize(object);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_Object_JxxNewRetainedBatch(benchmark::State& state) {
    initializeJxxOnce();
    const auto batchSize = static_cast<std::size_t>(state.range(0));
    std::vector<::jxx::Ptr<::jxx::lang::Object>> objects;
    objects.reserve(batchSize);

    for (auto _ : state) {
        objects.clear();
        for (std::size_t index = 0; index < batchSize; ++index) {
            objects.push_back(::jxx::NEW<::jxx::lang::Object>());
        }
        benchmark::DoNotOptimize(objects.data());
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(
        state.iterations() * static_cast<std::int64_t>(batchSize));
    state.SetBytesProcessed(
        state.iterations() * static_cast<std::int64_t>(
            batchSize * sizeof(::jxx::lang::Object)));
}

void BM_Object_GetClass(benchmark::State& state) {
    initializeJxxOnce();
    const auto object = ::jxx::NEW<::jxx::lang::Object>();
    for (auto _ : state) {
        const auto clazz = object->getClass();
        benchmark::DoNotOptimize(clazz);
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Object_JxxNew)
    ->Repetitions(kBenchmarkRepetitions)
    ->ReportAggregatesOnly(true)
    ->ComputeStatistics("p90", p90)
    ->ComputeStatistics("p95", p95)
    ->ComputeStatistics("p99", p99);

BENCHMARK(BM_Object_StdMakeSharedBaseline)
    ->Repetitions(kBenchmarkRepetitions)
    ->ReportAggregatesOnly(true)
    ->ComputeStatistics("p90", p90)
    ->ComputeStatistics("p95", p95)
    ->ComputeStatistics("p99", p99);

BENCHMARK(BM_Object_JxxNewRetainedBatch)
    ->Arg(1)
    ->Arg(16)
    ->Arg(256)
    ->Arg(4096)
    ->Repetitions(kBenchmarkRepetitions)
    ->ReportAggregatesOnly(true)
    ->ComputeStatistics("p90", p90)
    ->ComputeStatistics("p95", p95)
    ->ComputeStatistics("p99", p99);

BENCHMARK(BM_Object_GetClass)
    ->Repetitions(kBenchmarkRepetitions)
    ->ReportAggregatesOnly(true)
    ->ComputeStatistics("p90", p90)
    ->ComputeStatistics("p95", p95)
    ->ComputeStatistics("p99", p99);

} // namespace
