#include <gtest/gtest.h>
#include <limits>

#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

TEST(TimeUnitJava8Parity, ConvertsAndSaturates) {
    using ::jxx::util::concurrent::TimeUnit;
    EXPECT_EQ(1000, TimeUnit::SECONDS()->toMillis(1));
    EXPECT_EQ(1, TimeUnit::MILLISECONDS()->toSeconds(1999));
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jlong>::max(),
        TimeUnit::DAYS()->toNanos(std::numeric_limits<::jxx::lang::jlong>::max()));
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jlong>::min(),
        TimeUnit::DAYS()->toNanos(std::numeric_limits<::jxx::lang::jlong>::min()));
}

TEST(TimeUnitJava8Parity, RejectsNullReferences) {
    using ::jxx::util::concurrent::TimeUnit;
    EXPECT_ANY_THROW(TimeUnit::SECONDS()->convert(1, nullptr));
    EXPECT_ANY_THROW(TimeUnit::SECONDS()->timedWait(nullptr, 1));
    EXPECT_ANY_THROW(TimeUnit::SECONDS()->timedJoin(nullptr, 1));
}

TEST(TimeUnitJava8Parity, NonPositiveTimeoutReturnsWithoutWaiting) {
    using ::jxx::util::concurrent::TimeUnit;
    const auto object = ::jxx::NEW<::jxx::lang::Object>();
    EXPECT_NO_THROW(TimeUnit::SECONDS()->sleep(0));
    EXPECT_NO_THROW(TimeUnit::SECONDS()->sleep(-1));
    EXPECT_NO_THROW(TimeUnit::SECONDS()->timedWait(object, 0));
    EXPECT_NO_THROW(TimeUnit::SECONDS()->timedWait(object, -1));
}

} // namespace
