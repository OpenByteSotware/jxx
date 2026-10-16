#include <gtest/gtest.h>

#include "ext/xml/bind/jxx.ext.xml.bind.DatatypeConverter.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"

TEST(DatatypeConverterHexParity, PrintsUppercaseUnsignedBytes) {
    auto value = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*value)[0] = static_cast<::jxx::lang::jbyte>(0x00);
    (*value)[1] = static_cast<::jxx::lang::jbyte>(0x0F);
    (*value)[2] = static_cast<::jxx::lang::jbyte>(0x80);
    (*value)[3] = static_cast<::jxx::lang::jbyte>(0xFF);

    EXPECT_EQ("000F80FF",
        ::jxx::ext::xml::bind::DatatypeConverter::printHexBinary(value)->utf8());
}

TEST(DatatypeConverterHexParity, MatchesPortUdpRxMessageIdByteOrder) {
    auto data = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    (*data)[0] = static_cast<::jxx::lang::jbyte>(0x12);
    (*data)[1] = static_cast<::jxx::lang::jbyte>(0xAB);

    const auto text = ::jxx::ext::xml::bind::DatatypeConverter::printHexBinary(data);
    EXPECT_EQ("12AB", text->utf8());
}

TEST(DatatypeConverterHexParity, ParsesUpperAndLowerCase) {
    const auto result = ::jxx::ext::xml::bind::DatatypeConverter::parseHexBinary(
        ::jxx::NEW<::jxx::lang::String>("00aBfF"));
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(3, result->length);
    EXPECT_EQ(0, static_cast<unsigned char>((*result)[0]));
    EXPECT_EQ(0xAB, static_cast<unsigned char>((*result)[1]));
    EXPECT_EQ(0xFF, static_cast<unsigned char>((*result)[2]));
}

TEST(DatatypeConverterHexParity, RejectsOddLengthAndInvalidDigits) {
    EXPECT_THROW(
        ::jxx::ext::xml::bind::DatatypeConverter::parseHexBinary(
            ::jxx::NEW<::jxx::lang::String>("ABC")),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(
        ::jxx::ext::xml::bind::DatatypeConverter::parseHexBinary(
            ::jxx::NEW<::jxx::lang::String>("0Z")),
        ::jxx::lang::IllegalArgumentException);
}
