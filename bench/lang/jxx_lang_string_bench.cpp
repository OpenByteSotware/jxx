#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {

constexpr int kBenchmarkRepetitions = 15;

double percentile(const std::vector<double>& samples, double rank) {
    if (samples.empty()) return 0.0;
    auto sorted = samples;
    std::sort(sorted.begin(), sorted.end());
    if (sorted.size() == 1U) return sorted.front();
    const double position = rank * static_cast<double>(sorted.size() - 1U);
    const auto lower = static_cast<std::size_t>(position);
    const auto upper = std::min(lower + 1U, sorted.size() - 1U);
    const double fraction = position - static_cast<double>(lower);
    return sorted[lower] + ((sorted[upper] - sorted[lower]) * fraction);
}

double p90(const std::vector<double>& samples) { return percentile(samples, 0.90); }
double p95(const std::vector<double>& samples) { return percentile(samples, 0.95); }
double p99(const std::vector<double>& samples) { return percentile(samples, 0.99); }

void initializeJxxOnce() {
    static const bool initialized = [] {
        ::jxx::lang::initializeClasses();
        return true;
    }();
    benchmark::DoNotOptimize(initialized);
}

void BM_String_JxxNewUtf8(benchmark::State& state) {
    initializeJxxOnce();
    const std::string input(static_cast<std::size_t>(state.range(0)), 'x');
    for (auto _ : state) {
        auto value = ::jxx::NEW<::jxx::lang::String>(input);
        benchmark::DoNotOptimize(value);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_String_StdStringBaseline(benchmark::State& state) {
    const std::string input(static_cast<std::size_t>(state.range(0)), 'x');
    for (auto _ : state) {
        auto value = std::make_shared<std::string>(input);
        benchmark::DoNotOptimize(value);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_String_JxxCopyConstructor(benchmark::State& state) {
    initializeJxxOnce();
    const auto source = ::jxx::NEW<::jxx::lang::String>(
        std::string(static_cast<std::size_t>(state.range(0)), 'x'));
    for (auto _ : state) {
        auto value = ::jxx::NEW<::jxx::lang::String>(source);
        benchmark::DoNotOptimize(value);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_String_Equals(benchmark::State& state) {
    initializeJxxOnce();
    const std::string input(static_cast<std::size_t>(state.range(0)), 'x');
    const auto left = ::jxx::NEW<::jxx::lang::String>(input);
    const auto right = ::jxx::NEW<::jxx::lang::String>(input);
    const auto rightObject = ::jxx::CAST<::jxx::lang::Object>(right);
    for (auto _ : state) {
        const auto equal = left->equals(rightObject);
        benchmark::DoNotOptimize(equal);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_String_HashCodeCached(benchmark::State& state) {
    initializeJxxOnce();
    const auto value = ::jxx::NEW<::jxx::lang::String>(
        std::string(static_cast<std::size_t>(state.range(0)), 'x'));
    benchmark::DoNotOptimize(value->hashCode());
    for (auto _ : state) {
        const auto hash = value->hashCode();
        benchmark::DoNotOptimize(hash);
    }
    state.SetItemsProcessed(state.iterations());
}

#define JXX_STRING_STATS \
    ->Repetitions(kBenchmarkRepetitions) \
    ->ReportAggregatesOnly(true) \
    ->ComputeStatistics("p90", p90) \
    ->ComputeStatistics("p95", p95) \
    ->ComputeStatistics("p99", p99)

BENCHMARK(BM_String_JxxNewUtf8)->Arg(0)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_STATS;
BENCHMARK(BM_String_StdStringBaseline)->Arg(0)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_STATS;
BENCHMARK(BM_String_JxxCopyConstructor)->Arg(0)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_STATS;
BENCHMARK(BM_String_Equals)->Arg(0)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_STATS;
BENCHMARK(BM_String_HashCodeCached)->Arg(0)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_STATS;

#undef JXX_STRING_STATS

} // namespace
