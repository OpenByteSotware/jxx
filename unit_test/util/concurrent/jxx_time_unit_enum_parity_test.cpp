#include <gtest/gtest.h>

#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

TEST(TimeUnitEnumParity, IsAnEnumAndExposesEnumIdentitySemantics) {
    using ::jxx::util::concurrent::TimeUnit;

    const auto seconds = TimeUnit::SECONDS();
    const auto minutes = TimeUnit::MINUTES();

    ASSERT_NE(nullptr, ::jxx::CAST<::jxx::lang::Enum<TimeUnit>>(seconds));
    EXPECT_EQ(3, seconds->ordinal());
    EXPECT_TRUE(seconds->name()->equals(
        ::jxx::NEW<::jxx::lang::String>("SECONDS")));
    EXPECT_TRUE(seconds->equals(
        ::jxx::CAST<::jxx::lang::Object>(TimeUnit::SECONDS())));
    EXPECT_FALSE(seconds->equals(
        ::jxx::CAST<::jxx::lang::Object>(minutes)));
    EXPECT_LT(seconds->compareTo(minutes), 0);
    EXPECT_EQ(TimeUnit::Class().get(), seconds->getDeclaringClass().get());
}

TEST(TimeUnitEnumParity, ValuesReturnsFreshArrayWithCanonicalConstants) {
    using ::jxx::util::concurrent::TimeUnit;

    const auto first = TimeUnit::values();
    const auto second = TimeUnit::values();

    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_NE(first.get(), second.get());
    ASSERT_EQ(7U, first->length);
    EXPECT_EQ(TimeUnit::NANOSECONDS().get(), (*first)[0].get());
    EXPECT_EQ(TimeUnit::DAYS().get(), (*first)[6].get());
}

} // namespace
