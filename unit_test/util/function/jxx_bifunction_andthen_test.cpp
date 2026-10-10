#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.BiFunction.h"

namespace {
class Concat final
    : public ::jxx::lang::ClassBase<
          Concat,
          ::jxx::lang::Object,
          ::jxx::util::function::BiFunction<
              ::jxx::lang::String,
              ::jxx::lang::String,
              ::jxx::lang::String>> {
public:
    ::jxx::Ptr<::jxx::lang::String> apply(
        const ::jxx::Ptr<::jxx::lang::String>& left,
        const ::jxx::Ptr<::jxx::lang::String>& right) override {
        return ::jxx::NEW<::jxx::lang::String>(left->utf8() + right->utf8());
    }
};

class Length final
    : public ::jxx::lang::ClassBase<
          Length,
          ::jxx::lang::Object,
          ::jxx::util::function::Function<
              ::jxx::lang::String,
              ::jxx::lang::Integer>> {
public:
    ::jxx::Ptr<::jxx::lang::Integer> apply(
        const ::jxx::Ptr<::jxx::lang::String>& value) override {
        return ::jxx::NEW<::jxx::lang::Integer>(value->length());
    }
};
} // namespace

TEST(JxxBiFunctionAndThenParity, AppliesFirstFunctionThenAfterFunction) {
    auto first = ::jxx::CAST<::jxx::util::function::BiFunction<
        ::jxx::lang::String, ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<Concat>());
    auto after = ::jxx::CAST<::jxx::util::function::Function<
        ::jxx::lang::String, ::jxx::lang::Integer>>(
            ::jxx::NEW<Length>());

    auto combined = first->andThen<::jxx::lang::Integer>(after);
    auto result = combined->apply(
        ::jxx::NEW<::jxx::lang::String>("ab"),
        ::jxx::NEW<::jxx::lang::String>("cde"));

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->intValue(), 5);
}

TEST(JxxBiFunctionAndThenParity, RejectsNullAfterFunction) {
    auto first = ::jxx::CAST<::jxx::util::function::BiFunction<
        ::jxx::lang::String, ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<Concat>());
    EXPECT_THROW(
        first->andThen<::jxx::lang::Integer>(nullptr),
        ::jxx::lang::NullPointerException);
}
