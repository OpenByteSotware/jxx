#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.BinaryOperator.h"
#include "util/function/jxx.util.function.BiPredicate.h"

TEST(JxxFunctionRemainingInterfaces, MetadataAndHierarchy)
{
    using Operator =
        ::jxx::util::function::BinaryOperator<::jxx::lang::String>;

    using Function =
        ::jxx::util::function::BiFunction<
        ::jxx::lang::String,
        ::jxx::lang::String,
        ::jxx::lang::String>;

    using Predicate =
        ::jxx::util::function::BiPredicate<
        ::jxx::lang::String,
        ::jxx::lang::String>;

    EXPECT_TRUE(Operator::Class()->isInterface());
    EXPECT_TRUE(
        Function::Class()->isAssignableFrom(Operator::Class()));
    EXPECT_TRUE(Predicate::Class()->isInterface());
}