#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.Function.h"

namespace {

class StringLength final
    : public ::jxx::lang::ClassBase<
          StringLength,
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

class IntegerText final
    : public ::jxx::lang::ClassBase<
          IntegerText,
          ::jxx::lang::Object,
          ::jxx::util::function::Function<
              ::jxx::lang::Integer,
              ::jxx::lang::String>> {
public:
    ::jxx::Ptr<::jxx::lang::String> apply(
        const ::jxx::Ptr<::jxx::lang::Integer>& value) override {
        return value->toString();
    }
};

} // namespace

TEST(JxxFunctionCompositionParity, ComposeAppliesBeforeThenReceiver) {
    auto length = ::jxx::CAST<::jxx::util::function::Function<
        ::jxx::lang::String,
        ::jxx::lang::Integer>>(::jxx::NEW<StringLength>());
    auto text = ::jxx::CAST<::jxx::util::function::Function<
        ::jxx::lang::Integer,
        ::jxx::lang::String>>(::jxx::NEW<IntegerText>());

    auto composed = text->compose<::jxx::lang::String>(length);
    EXPECT_EQ(
        composed->apply(::jxx::NEW<::jxx::lang::String>("abcd"))->utf8(),
        "4");
}

TEST(JxxFunctionCompositionParity, AndThenAppliesReceiverThenAfter) {
    auto length = ::jxx::CAST<::jxx::util::function::Function<
        ::jxx::lang::String,
        ::jxx::lang::Integer>>(::jxx::NEW<StringLength>());
    auto text = ::jxx::CAST<::jxx::util::function::Function<
        ::jxx::lang::Integer,
        ::jxx::lang::String>>(::jxx::NEW<IntegerText>());

    auto chained = length->andThen<::jxx::lang::String>(text);
    EXPECT_EQ(
        chained->apply(::jxx::NEW<::jxx::lang::String>("abc"))->utf8(),
        "3");
}

TEST(JxxFunctionCompositionParity, IdentityReturnsSameReference) {
    auto identity = ::jxx::util::function::Function<
        ::jxx::lang::String,
        ::jxx::lang::String>::identity();
    auto value = ::jxx::NEW<::jxx::lang::String>("value");

    EXPECT_EQ(identity->apply(value), value);
    EXPECT_EQ(identity->apply(nullptr), nullptr);
}

TEST(JxxFunctionCompositionParity, RejectsNullFunctions) {
    auto length = ::jxx::CAST<::jxx::util::function::Function<
        ::jxx::lang::String,
        ::jxx::lang::Integer>>(::jxx::NEW<StringLength>());

    EXPECT_THROW(
        length->compose<::jxx::lang::String>(nullptr),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        length->andThen<::jxx::lang::String>(nullptr),
        ::jxx::lang::NullPointerException);
}
