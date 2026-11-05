#include <gtest/gtest.h>

#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.String.h"

TEST(ProcessBuilderRedirectTypeTest, ExposesEnumObjectSemantics) {
    using Type = ::jxx::lang::ProcessBuilder::Redirect::Type;
    const auto values = Type::values();
    ASSERT_NE(nullptr, values);
    ASSERT_EQ(5U, values->length);
    EXPECT_EQ(Type::PIPE, (*values)[0]);
    EXPECT_EQ(Type::INHERIT, (*values)[1]);
    EXPECT_EQ(Type::READ, (*values)[2]);
    EXPECT_EQ(Type::WRITE, (*values)[3]);
    EXPECT_EQ(Type::APPEND, (*values)[4]);
    EXPECT_EQ(0, Type::PIPE->ordinal());
    EXPECT_EQ(4, Type::APPEND->ordinal());
}

TEST(ProcessBuilderRedirectTypeTest, ValueOfReturnsCanonicalConstant) {
    using Type = ::jxx::lang::ProcessBuilder::Redirect::Type;
    EXPECT_EQ(Type::READ, Type::valueOf(
        ::jxx::NEW<::jxx::lang::String>("READ")));
}

TEST(ProcessBuilderRedirectTypeTest, RedirectTypeUsesEnumObject) {
    using Redirect = ::jxx::lang::ProcessBuilder::Redirect;
    EXPECT_EQ(Redirect::Type::PIPE, Redirect::PIPE->type());
    EXPECT_EQ(Redirect::Type::INHERIT, Redirect::INHERIT->type());
}
