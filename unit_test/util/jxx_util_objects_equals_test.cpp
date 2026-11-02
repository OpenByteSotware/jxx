#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Objects.h"

namespace {

TEST(ObjectsEqualsParity, ConvertedGenericReferencesCanCallPublicEquals) {
    auto first = ::jxx::NEW<::jxx::lang::String>("same");
    auto second = ::jxx::NEW<::jxx::lang::String>("same");
    auto different = ::jxx::NEW<::jxx::lang::String>("different");

    EXPECT_TRUE(::jxx::util::Objects::equals(first, second));
    EXPECT_FALSE(::jxx::util::Objects::equals(first, different));
}

TEST(ObjectsEqualsParity, EqualsIsNullSafe) {
    ::jxx::Ptr<::jxx::lang::String> nullString;
    auto value = ::jxx::NEW<::jxx::lang::String>("value");

    EXPECT_TRUE(::jxx::util::Objects::equals(nullString, nullString));
    EXPECT_FALSE(::jxx::util::Objects::equals(value, nullString));
    EXPECT_FALSE(::jxx::util::Objects::equals(nullString, value));
}

} // namespace
