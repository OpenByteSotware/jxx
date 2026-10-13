#include <gtest/gtest.h>
#include <limits>

#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

TEST(TimeUnitDirectConversionParity, LargeCoarseValuesDoNotSaturateThroughNanos) {
    using namespace ::jxx::util::concurrent;
    const auto maximum = std::numeric_limits<::jxx::lang::jlong>::max();
    EXPECT_EQ(maximum, TimeUnit::DAYS()->toDays(maximum));
    EXPECT_EQ(maximum / 24, TimeUnit::HOURS()->toDays(maximum));
    EXPECT_EQ(maximum / 60, TimeUnit::MINUTES()->toHours(maximum));
    EXPECT_EQ(maximum / 1000, TimeUnit::MILLISECONDS()->toSeconds(maximum));
}

TEST(TimeUnitDirectConversionParity, FinerConversionsSaturateAtBounds) {
    using namespace ::jxx::util::concurrent;
    const auto maximum = std::numeric_limits<::jxx::lang::jlong>::max();
    const auto minimum = std::numeric_limits<::jxx::lang::jlong>::min();
    EXPECT_EQ(maximum, TimeUnit::DAYS()->toMillis(maximum));
    EXPECT_EQ(minimum, TimeUnit::DAYS()->toMillis(minimum));
    EXPECT_EQ(maximum, TimeUnit::SECONDS()->toMicros(maximum));
}

TEST(TimeUnitDirectConversionParity, SubMillisecondConversionsRetainRemainder) {
    using namespace ::jxx::util::concurrent;
    EXPECT_EQ(0, TimeUnit::NANOSECONDS()->toMillis(999999));
    EXPECT_EQ(1, TimeUnit::NANOSECONDS()->toMillis(1000000));
    EXPECT_EQ(999, TimeUnit::NANOSECONDS()->toMicros(999999));
}

} // namespace
