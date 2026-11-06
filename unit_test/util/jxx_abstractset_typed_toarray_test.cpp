#include <gtest/gtest.h>
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/jxx.util.HashSet.h"

TEST(AbstractSetTypedToArrayTest, TypedOverloadRemainsVisible) {
    auto values = ::jxx::NEW<::jxx::util::HashSet<::jxx::lang::Integer>>();
    values->add(::jxx::lang::Integer::valueOf(1));
    auto destination = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::Integer>, 1U>>(0);
    const auto result = values->toArray(destination);
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(1, result->length);
    EXPECT_EQ(1, (*result)[0]->intValue());
}
