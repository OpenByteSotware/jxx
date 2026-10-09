#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {
TEST(SystemPropertyParity, DistinctEqualStringKeyFindsAndClearsProperty) {
    const auto storedKey = ::jxx::NEW<::jxx::lang::String>(
        "jxx.test.distinct.string.key");
    const auto lookupKey = ::jxx::NEW<::jxx::lang::String>(
        "jxx.test.distinct.string.key");

    ASSERT_NE(storedKey.get(), lookupKey.get());
    (void)::jxx::lang::System::setProperty(
        storedKey,
        ::jxx::NEW<::jxx::lang::String>("value"));

    const auto value = ::jxx::lang::System::getProperty(lookupKey);
    ASSERT_NE(nullptr, value);
    EXPECT_EQ("value", value->utf8());

    const auto removed = ::jxx::lang::System::clearProperty(lookupKey);
    ASSERT_NE(nullptr, removed);
    EXPECT_EQ("value", removed->utf8());
    EXPECT_EQ(nullptr, ::jxx::lang::System::getProperty(storedKey));
}
} // namespace
