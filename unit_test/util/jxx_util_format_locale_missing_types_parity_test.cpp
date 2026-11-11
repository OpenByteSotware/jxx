#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.FormattableFlags.h"
#include "util/jxx.util.IllformedLocaleException.h"
#include "util/jxx.util.MissingFormatWidthException.h"

TEST(FormattableFlagsParity, DefinesJava8BitFlags) {
    EXPECT_EQ(1, ::jxx::util::FormattableFlags::LEFT_JUSTIFY);
    EXPECT_EQ(2, ::jxx::util::FormattableFlags::UPPERCASE);
    EXPECT_EQ(4, ::jxx::util::FormattableFlags::ALTERNATE);
}

TEST(MissingFormatWidthExceptionParity, PreservesFormatSpecifier) {
    const auto specifier = ::jxx::NEW<::jxx::lang::String>("%-s");
    const auto exception =
        ::jxx::NEW<::jxx::util::MissingFormatWidthException>(specifier);
    EXPECT_TRUE(exception->getFormatSpecifier()->equals(specifier));
    EXPECT_TRUE(exception->getMessage()->equals(specifier));
}

TEST(IllformedLocaleExceptionParity, PreservesErrorIndex) {
    const auto message = ::jxx::NEW<::jxx::lang::String>("bad locale");
    const auto indexed =
        ::jxx::NEW<::jxx::util::IllformedLocaleException>(message, 3);
    EXPECT_EQ(3, indexed->getErrorIndex());

    const auto unindexed =
        ::jxx::NEW<::jxx::util::IllformedLocaleException>(message);
    EXPECT_EQ(-1, unindexed->getErrorIndex());
}
