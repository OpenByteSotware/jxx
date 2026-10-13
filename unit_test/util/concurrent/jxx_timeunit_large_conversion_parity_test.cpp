#include <gtest/gtest.h>
#include <limits>

#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

TEST(TimeUnitLargeConversionParity, CoarseToCoarseConversionDoesNotRouteThroughNanos) {
    using namespace ::jxx::util::concurrent;
    const auto maximum = std::numeric_limits<::jxx::lang::jlong>::max();
    EXPECT_EQ(maximum / 24,
        TimeUnit::DAYS()->convert(maximum, TimeUnit::HOURS()));
    EXPECT_EQ(maximum / 60,
        TimeUnit::HOURS()->convert(maximum, TimeUnit::MINUTES()));
}

TEST(TimeUnitLargeConversionParity, FinerTargetSaturatesAtLongBounds) {
    using namespace ::jxx::util::concurrent;
    const auto maximum = std::numeric_limits<::jxx::lang::jlong>::max();
    const auto minimum = std::numeric_limits<::jxx::lang::jlong>::min();
    EXPECT_EQ(maximum,
        TimeUnit::NANOSECONDS()->convert(maximum, TimeUnit::DAYS()));
    EXPECT_EQ(minimum,
        TimeUnit::NANOSECONDS()->convert(minimum, TimeUnit::DAYS()));
}

TEST(TimeUnitLargeConversionParity, SameUnitPreservesExactLongValue) {
    using namespace ::jxx::util::concurrent;
    const auto minimum = std::numeric_limits<::jxx::lang::jlong>::min();
    EXPECT_EQ(minimum,
        TimeUnit::SECONDS()->convert(minimum, TimeUnit::SECONDS()));
}

} // namespace
