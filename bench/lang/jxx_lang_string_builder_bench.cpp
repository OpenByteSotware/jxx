#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"

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

void BM_StringBuilder_JxxNew(benchmark::State& state) {
    initializeJxxOnce();
    for (auto _ : state) {
        auto builder = ::jxx::NEW<::jxx::lang::StringBuilder>();
        benchmark::DoNotOptimize(builder);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_StringBuilder_AppendChars(benchmark::State& state) {
    initializeJxxOnce();
    const auto count = static_cast<::jxx::lang::jint>(state.range(0));
    for (auto _ : state) {
        auto builder = ::jxx::NEW<::jxx::lang::StringBuilder>(count);
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            builder->appendSB(static_cast<::jxx::lang::jchar>('x'));
        }
        benchmark::DoNotOptimize(builder);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * count);
}

void BM_StringBuilder_AppendString(benchmark::State& state) {
    initializeJxxOnce();
    const auto value = ::jxx::NEW<::jxx::lang::String>("benchmark");
    const auto count = static_cast<::jxx::lang::jint>(state.range(0));
    for (auto _ : state) {
        auto builder = ::jxx::NEW<::jxx::lang::StringBuilder>(count * value->length());
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            builder->append(value);
        }
        benchmark::DoNotOptimize(builder);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * count);
}

void BM_StringBuilder_AppendCharsAndToString(benchmark::State& state) {
    initializeJxxOnce();
    const auto count = static_cast<::jxx::lang::jint>(state.range(0));
    for (auto _ : state) {
        auto builder = ::jxx::NEW<::jxx::lang::StringBuilder>(count);
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            builder->appendSB(static_cast<::jxx::lang::jchar>('x'));
        }
        const auto result = builder->toString();
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * count);
}

void BM_StringBuilder_StdStringAppendBaseline(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    for (auto _ : state) {
        std::string value;
        value.reserve(count);
        for (std::size_t index = 0; index < count; ++index) value.push_back('x');
        benchmark::DoNotOptimize(value);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(count));
}

#define JXX_STRING_BUILDER_STATS \
    ->Repetitions(kBenchmarkRepetitions) \
    ->ReportAggregatesOnly(true) \
    ->ComputeStatistics("p90", p90) \
    ->ComputeStatistics("p95", p95) \
    ->ComputeStatistics("p99", p99)

BENCHMARK(BM_StringBuilder_JxxNew) JXX_STRING_BUILDER_STATS;
BENCHMARK(BM_StringBuilder_AppendChars)->Arg(1)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_BUILDER_STATS;
BENCHMARK(BM_StringBuilder_AppendString)->Arg(1)->Arg(16)->Arg(256) JXX_STRING_BUILDER_STATS;
BENCHMARK(BM_StringBuilder_AppendCharsAndToString)->Arg(1)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_BUILDER_STATS;
BENCHMARK(BM_StringBuilder_StdStringAppendBaseline)->Arg(1)->Arg(16)->Arg(256)->Arg(4096) JXX_STRING_BUILDER_STATS;

#undef JXX_STRING_BUILDER_STATS

} // namespace
