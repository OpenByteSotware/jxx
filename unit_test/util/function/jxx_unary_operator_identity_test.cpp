#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.UnaryOperator.h"

TEST(JxxUnaryOperatorIdentityParity, ReturnsSameReference) {
    auto identity = ::jxx::util::function::UnaryOperator<
        ::jxx::lang::String>::identity();
    auto value = ::jxx::NEW<::jxx::lang::String>("value");

    ASSERT_NE(identity, nullptr);
    EXPECT_EQ(identity->apply(value), value);
    EXPECT_EQ(identity->apply(nullptr), nullptr);
}

TEST(JxxUnaryOperatorIdentityParity, PreservesFunctionHierarchy) {
    using Operator =
        ::jxx::util::function::UnaryOperator<::jxx::lang::String>;
    using Function =
        ::jxx::util::function::Function<
            ::jxx::lang::String,
            ::jxx::lang::String>;

    EXPECT_TRUE(Operator::Class()->isInterface());
    EXPECT_TRUE(Function::Class()->isAssignableFrom(Operator::Class()));
}
