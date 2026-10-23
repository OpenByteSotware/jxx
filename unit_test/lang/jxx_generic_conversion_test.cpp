#include <gtest/gtest.h>

#include <initializer_list>
#include <type_traits>
#include <utility>

#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"

namespace {

template <typename T>
class UserGeneric final {
public:
    void add(const T& value) {
        value_ = value;
    }

    template <typename Source,
        std::enable_if_t<
            ::jxx::is_generic_convertible_v<T, Source&&> &&
            !std::is_same_v<std::decay_t<Source>, T>,
            int> = 0>
    void add(Source&& value) {
        add(::jxx::CONVERT<T>(std::forward<Source>(value)));
    }

    const T& value() const noexcept {
        return value_;
    }

private:
    T value_{};
};

TEST(GenericConversionTest, BoxesEveryPrimitiveWrapper) {
    EXPECT_EQ(1, ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Byte>>(
        static_cast<::jxx::lang::jbyte>(1))->byteValue());
    EXPECT_EQ(2, ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Short>>(
        static_cast<::jxx::lang::jshort>(2))->shortValue());
    EXPECT_EQ(3, ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Integer>>(3)->intValue());
    EXPECT_EQ(4, ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Long>>(
        static_cast<::jxx::lang::jlong>(4))->longValue());
    EXPECT_FLOAT_EQ(5.0F,
        ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Float>>(5.0F)->floatValue());
    EXPECT_DOUBLE_EQ(6.0,
        ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Double>>(6.0)->doubleValue());
    EXPECT_TRUE(
        ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Boolean>>(true)->booleanValue());
    EXPECT_EQ(u'Z',
        ::jxx::CONVERT<::jxx::Ptr<::jxx::lang::Character>>(u'Z')->charValue());
}

TEST(GenericConversionTest, ConvertsStringLiteralsAndNull) {
    UserGeneric<::jxx::Ptr<::jxx::lang::String>> values;
    values.add("hello");
    ASSERT_NE(nullptr, values.value());
    EXPECT_EQ("hello", values.value()->utf8());

    values.add(nullptr);
    EXPECT_EQ(nullptr, values.value());
}

TEST(GenericConversionTest, SupportsUserGenericIntegerParameter) {
    UserGeneric<::jxx::Ptr<::jxx::lang::Integer>> values;
    values.add(42);
    ASSERT_NE(nullptr, values.value());
    EXPECT_EQ(42, values.value()->intValue());
}

TEST(GenericConversionTest, SupportsBoxedJxxArrayInitializer) {
    using Element = ::jxx::Ptr<::jxx::lang::Integer>;
    using Init = ::jxx::GenericArgument<Element>;

    auto values = ::jxx::NEW<::jxx::lang::JxxArray<Element, 1U>>(
        std::initializer_list<Init>{1, nullptr, 2});

    ASSERT_NE(nullptr, values);
    ASSERT_EQ(3, values->length);
    ASSERT_NE(nullptr, (*values)[0]);
    EXPECT_EQ(1, (*values)[0]->intValue());
    EXPECT_EQ(nullptr, (*values)[1]);
    ASSERT_NE(nullptr, (*values)[2]);
    EXPECT_EQ(2, (*values)[2]->intValue());
}

static_assert(!::jxx::is_generic_convertible_v<
    ::jxx::Ptr<::jxx::lang::Boolean>, ::jxx::lang::jint>,
    "Integer values must not implicitly box as Boolean");

} // namespace
