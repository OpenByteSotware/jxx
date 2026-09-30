#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
namespace {

TEST(StringValueOfTest, NullObjectReturnsNullText) {
    const ::jxx::Ptr<Object> value;

    const auto result = String::valueOf(value);

    ASSERT_NE(nullptr, result);
    EXPECT_EQ("null", result->utf8());
}

TEST(StringValueOfTest, StringObjectReturnsSameString) {
    const auto text = ::jxx::NEW<String>("hello world");
    const ::jxx::Ptr<Object> value = text;

    const auto result = String::valueOf(value);

    ASSERT_NE(nullptr, result);
    EXPECT_EQ(text.get(), result.get());
    EXPECT_EQ("hello world", result->utf8());
}

TEST(StringValueOfTest, NonStringObjectUsesToString) {
    const auto integer = ::jxx::NEW<Integer>(42);
    const ::jxx::Ptr<Object> value = integer;

    const auto result = String::valueOf(value);

    ASSERT_NE(nullptr, result);
    EXPECT_EQ("42", result->utf8());
}

} // namespace
} // namespace jxx::lang
