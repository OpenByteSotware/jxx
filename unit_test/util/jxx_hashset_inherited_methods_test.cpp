#include <gtest/gtest.h>
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/jxx.util.HashSet.h"

TEST(HashSetInheritedMethodsTest, TypedToArrayIsVisible)
{
    auto values = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>();
    auto target = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::Integer>, 1U>>(0);
    const auto result = values->toArray(target);
    EXPECT_NE(nullptr, result);
}
