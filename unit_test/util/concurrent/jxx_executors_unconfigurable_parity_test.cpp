#include <gtest/gtest.h>

#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"

namespace {

TEST(ExecutorsJava8Parity, UnconfigurableExecutorDelegatesWithoutExposingImplementation) {
    using namespace ::jxx::util::concurrent;
    const auto concrete = Executors::newFixedThreadPool(1);
    const auto wrapped = Executors::unconfigurableExecutorService(concrete);
    ASSERT_NE(nullptr, wrapped);
    EXPECT_EQ(nullptr, ::jxx::CAST<ThreadPoolExecutor>(wrapped));
    EXPECT_FALSE(wrapped->isShutdown());
    wrapped->shutdown();
    EXPECT_TRUE(wrapped->isShutdown());
}

TEST(ExecutorsJava8Parity, UnconfigurableScheduledExecutorDelegatesWithoutExposingImplementation) {
    using namespace ::jxx::util::concurrent;
    const auto concrete = Executors::newScheduledThreadPool(1);
    const auto wrapped =
        Executors::unconfigurableScheduledExecutorService(concrete);
    ASSERT_NE(nullptr, wrapped);
    EXPECT_EQ(nullptr, ::jxx::CAST<ScheduledThreadPoolExecutor>(wrapped));
    EXPECT_FALSE(wrapped->isShutdown());
    wrapped->shutdown();
    EXPECT_TRUE(wrapped->isShutdown());
}

TEST(ExecutorsJava8Parity, UnconfigurableFactoriesRejectNull) {
    using namespace ::jxx::util::concurrent;
    EXPECT_ANY_THROW(Executors::unconfigurableExecutorService(nullptr));
    EXPECT_ANY_THROW(
        Executors::unconfigurableScheduledExecutorService(nullptr));
}

} // namespace
