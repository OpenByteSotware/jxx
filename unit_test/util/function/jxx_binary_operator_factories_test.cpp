#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.BinaryOperator.h"

namespace {
class StringLengthComparator final
    : public ::jxx::lang::ClassBase<
          StringLengthComparator,
          ::jxx::lang::Object,
          ::jxx::util::Comparator<::jxx::lang::String>> {
public:
    ::jxx::lang::jint compare(
        const ::jxx::Ptr<::jxx::lang::String>& first,
        const ::jxx::Ptr<::jxx::lang::String>& second) const override {
        return first->length() - second->length();
    }
};
}

TEST(JxxBinaryOperatorFactoriesParity, MinByAndMaxByUseComparator) {
    auto comparator = ::jxx::CAST<
        ::jxx::util::Comparator<::jxx::lang::String>>(
            ::jxx::NEW<StringLengthComparator>());
    auto shortValue = ::jxx::NEW<::jxx::lang::String>("a");
    auto longValue = ::jxx::NEW<::jxx::lang::String>("abcd");

    auto minimum = ::jxx::util::function::BinaryOperator<
        ::jxx::lang::String>::minBy(comparator);
    auto maximum = ::jxx::util::function::BinaryOperator<
        ::jxx::lang::String>::maxBy(comparator);

    EXPECT_EQ(minimum->apply(shortValue, longValue), shortValue);
    EXPECT_EQ(maximum->apply(shortValue, longValue), longValue);
}

TEST(JxxBinaryOperatorFactoriesParity, TiesReturnFirstOperand) {
    auto comparator = ::jxx::CAST<
        ::jxx::util::Comparator<::jxx::lang::String>>(
            ::jxx::NEW<StringLengthComparator>());
    auto first = ::jxx::NEW<::jxx::lang::String>("ab");
    auto second = ::jxx::NEW<::jxx::lang::String>("cd");

    EXPECT_EQ(
        ::jxx::util::function::BinaryOperator<
            ::jxx::lang::String>::minBy(comparator)->apply(first, second),
        first);
    EXPECT_EQ(
        ::jxx::util::function::BinaryOperator<
            ::jxx::lang::String>::maxBy(comparator)->apply(first, second),
        first);
}

TEST(JxxBinaryOperatorFactoriesParity, RejectsNullComparator) {
    using Operator =
        ::jxx::util::function::BinaryOperator<::jxx::lang::String>;
    EXPECT_THROW(Operator::minBy(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(Operator::maxBy(nullptr), ::jxx::lang::NullPointerException);
}
