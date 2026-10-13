#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

TEST(TimeUnitEnumParity, ValuesAreInDeclarationOrderAndDefensivelyCopied) {
    using ::jxx::util::concurrent::TimeUnit;
    const auto first = TimeUnit::values();
    const auto second = TimeUnit::values();
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_NE(first, second);
    ASSERT_EQ(7, first->length);
    EXPECT_EQ(TimeUnit::NANOSECONDS(), (*first)[0]);
    EXPECT_EQ(TimeUnit::MICROSECONDS(), (*first)[1]);
    EXPECT_EQ(TimeUnit::MILLISECONDS(), (*first)[2]);
    EXPECT_EQ(TimeUnit::SECONDS(), (*first)[3]);
    EXPECT_EQ(TimeUnit::MINUTES(), (*first)[4]);
    EXPECT_EQ(TimeUnit::HOURS(), (*first)[5]);
    EXPECT_EQ(TimeUnit::DAYS(), (*first)[6]);
}

TEST(TimeUnitEnumParity, ValueOfNameOrdinalAndToString) {
    using ::jxx::util::concurrent::TimeUnit;
    const auto seconds = TimeUnit::valueOf(
        ::jxx::NEW<::jxx::lang::String>("SECONDS"));
    EXPECT_EQ(TimeUnit::SECONDS(), seconds);
    EXPECT_EQ(3, seconds->ordinal());
    EXPECT_EQ("SECONDS", seconds->name()->utf8());
    EXPECT_EQ("SECONDS", seconds->toString()->utf8());
}

TEST(TimeUnitEnumParity, ValueOfRejectsNullUnknownAndWrongCase) {
    using ::jxx::util::concurrent::TimeUnit;
    EXPECT_ANY_THROW(TimeUnit::valueOf(nullptr));
    EXPECT_ANY_THROW(TimeUnit::valueOf(
        ::jxx::NEW<::jxx::lang::String>("seconds")));
    EXPECT_ANY_THROW(TimeUnit::valueOf(
        ::jxx::NEW<::jxx::lang::String>("WEEKS")));
}

} // namespace
