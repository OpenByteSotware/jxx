#include <gtest/gtest.h>

#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"

namespace {

TEST(ExecutorsJava8Parity, SingleThreadExecutorDoesNotExposeThreadPoolExecutor) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadExecutor();
    ASSERT_NE(nullptr, executor);
    EXPECT_EQ(nullptr, ::jxx::CAST<ThreadPoolExecutor>(executor));
    executor->shutdown();
}

TEST(ExecutorsJava8Parity, SingleThreadScheduledExecutorDoesNotExposeScheduledPool) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    ASSERT_NE(nullptr, executor);
    EXPECT_EQ(nullptr, ::jxx::CAST<ScheduledThreadPoolExecutor>(executor));
    executor->shutdown();
}

TEST(ExecutorsJava8Parity, ThreadFactorySingleThreadFactoriesAlsoReturnWrappers) {
    using namespace ::jxx::util::concurrent;
    const auto factory = Executors::defaultThreadFactory();
    const auto executor = Executors::newSingleThreadExecutor(factory);
    const auto scheduled =
        Executors::newSingleThreadScheduledExecutor(factory);
    EXPECT_EQ(nullptr, ::jxx::CAST<ThreadPoolExecutor>(executor));
    EXPECT_EQ(nullptr, ::jxx::CAST<ScheduledThreadPoolExecutor>(scheduled));
    executor->shutdown();
    scheduled->shutdown();
}

} // namespace
