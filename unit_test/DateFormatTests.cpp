#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.DateFormat.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.TimeZone.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::lang::String;
using jxx::util::DateFormat;
using jxx::util::Locale;
using jxx::util::TimeZone;

TEST(DateFormatTests, FormatsEpochInUtcLikeZoneWithoutCrashing) {
    auto tz = TimeZone::getTimeZone(String::valueOf("UTC"));
    ASSERT_TRUE(tz != nullptr);

    auto df = DateFormat::ofPattern(String::valueOf("yyyy-MM-dd HH:mm:ss z"), Locale::US);
    auto text = df->format(0, tz);

    ASSERT_TRUE(text != nullptr);
    EXPECT_FALSE(text->isEmpty());
    ExpectStringContains(text, "1970");
}

TEST(DateFormatTests, FormatsWithDstAwareTimezoneApi) {
    auto tz = TimeZone::getTimeZone(String::valueOf("America/New_York"));
    ASSERT_TRUE(tz != nullptr);

    auto df = DateFormat::ofPattern(String::valueOf("yyyy-MM-dd HH:mm:ss z"), Locale::US);
    auto winter = df->format(1735689600000LL, tz); // 2025-01-01T00:00:00Z
    auto summer = df->format(1751328000000LL, tz); // 2025-07-01T00:00:00Z

    ASSERT_TRUE(winter != nullptr);
    ASSERT_TRUE(summer != nullptr);
    EXPECT_FALSE(winter->isEmpty());
    EXPECT_FALSE(summer->isEmpty());
}

TEST(DateFormatTests, ZoneAbbreviationOrIdIsIncludedForPatternZ) {
    auto tz = TimeZone::getTimeZone(String::valueOf("America/New_York"));
    ASSERT_TRUE(tz != nullptr);
    auto df = DateFormat::ofPattern(String::valueOf("z"), Locale::US);
    auto text = df->format(1735689600000LL, tz);
    ASSERT_TRUE(text != nullptr);
    EXPECT_FALSE(text->isEmpty());
}

} // namespace
