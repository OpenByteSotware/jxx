#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Formatter.h"

TEST(FormatterBigDecimalLayoutFormParity, DefinesJava8ConstantsInOrder) {
    using Form = ::jxx::util::Formatter::BigDecimalLayoutForm;

    ASSERT_NE(nullptr, Form::SCIENTIFIC);
    ASSERT_NE(nullptr, Form::DECIMAL_FLOAT);
    EXPECT_EQ(0, Form::SCIENTIFIC->ordinal());
    EXPECT_EQ(1, Form::DECIMAL_FLOAT->ordinal());
    EXPECT_EQ("SCIENTIFIC", Form::SCIENTIFIC->name()->utf8());
    EXPECT_EQ("DECIMAL_FLOAT", Form::DECIMAL_FLOAT->name()->utf8());
}

TEST(FormatterBigDecimalLayoutFormParity, ValuesReturnsDefensiveArray) {
    using Form = ::jxx::util::Formatter::BigDecimalLayoutForm;

    const auto first = Form::values();
    const auto second = Form::values();
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_NE(first.get(), second.get());
    ASSERT_EQ(2, first->length);
    EXPECT_EQ(Form::SCIENTIFIC.get(), (*first)[0].get());
    EXPECT_EQ(Form::DECIMAL_FLOAT.get(), (*first)[1].get());
}

TEST(FormatterBigDecimalLayoutFormParity, ValueOfReturnsCanonicalConstant) {
    using Form = ::jxx::util::Formatter::BigDecimalLayoutForm;

    const auto scientific = Form::valueOf(
        ::jxx::NEW<::jxx::lang::String>("SCIENTIFIC"));
    const auto decimalFloat = Form::valueOf(
        ::jxx::NEW<::jxx::lang::String>("DECIMAL_FLOAT"));

    EXPECT_EQ(Form::SCIENTIFIC.get(), scientific.get());
    EXPECT_EQ(Form::DECIMAL_FLOAT.get(), decimalFloat.get());
}
