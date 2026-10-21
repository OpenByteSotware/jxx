#include <gtest/gtest.h>

#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {
using ObjectArray = ::jxx::JxxArray<
    ::jxx::Ptr<::jxx::lang::Object>, 1U>;

::jxx::Ptr<::jxx::lang::String> formatByte(
    ::jxx::lang::jbyte value) {
    auto arguments = ::jxx::NEW<ObjectArray>(1);
    (*arguments)(0) = ::jxx::NEW<::jxx::lang::Byte>(value);
    return ::jxx::lang::String::format(
        ::jxx::NEW<::jxx::lang::String>("%02x-"),
        arguments);
}
}

TEST(DebugHexStringParity, FormatsPrimitiveByteRangeAsSourceRequires) {
    EXPECT_EQ("00-", formatByte(static_cast<::jxx::lang::jbyte>(0x00))->utf8());
    EXPECT_EQ("0f-", formatByte(static_cast<::jxx::lang::jbyte>(0x0F))->utf8());
    EXPECT_EQ("7f-", formatByte(static_cast<::jxx::lang::jbyte>(0x7F))->utf8());
    EXPECT_EQ("80-", formatByte(static_cast<::jxx::lang::jbyte>(0x80))->utf8());
    EXPECT_EQ("ff-", formatByte(static_cast<::jxx::lang::jbyte>(0xFF))->utf8());
}

TEST(DebugHexStringParity, FormatsPrimitiveByteArraySequence) {
    auto data = ::jxx::NEW<::jxx::lang::ByteArrayType>(5);
    (*data)[0] = static_cast<::jxx::lang::jbyte>(0x00);
    (*data)[1] = static_cast<::jxx::lang::jbyte>(0x0F);
    (*data)[2] = static_cast<::jxx::lang::jbyte>(0x7F);
    (*data)[3] = static_cast<::jxx::lang::jbyte>(0x80);
    (*data)[4] = static_cast<::jxx::lang::jbyte>(0xFF);

    std::string result;
    for (::jxx::lang::jint index = 0; index < data->length; ++index) {
        result += formatByte((*data)[index])->utf8();
    }
    EXPECT_EQ("00-0f-7f-80-ff-", result);
}

TEST(DebugHexStringParity, FormatsBoxedByteArraySequence) {
    using ByteObjectArray = ::jxx::JxxArray<
        ::jxx::Ptr<::jxx::lang::Byte>, 1U>;
    auto data = ::jxx::NEW<ByteObjectArray>(5);
    (*data)(0) = ::jxx::NEW<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(0x00));
    (*data)(1) = ::jxx::NEW<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(0x0F));
    (*data)(2) = ::jxx::NEW<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(0x7F));
    (*data)(3) = ::jxx::NEW<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(0x80));
    (*data)(4) = ::jxx::NEW<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(0xFF));

    std::string result;
    for (::jxx::lang::jint index = 0; index < (*data).size(); ++index) {
        auto arguments = ::jxx::NEW<ObjectArray>(1);
        (*arguments)(0) = ::jxx::CAST<::jxx::lang::Object>((*data)(index));
        result += ::jxx::lang::String::format(
            ::jxx::NEW<::jxx::lang::String>("%02x-"),
            arguments)->utf8();
    }
    EXPECT_EQ("00-0f-7f-80-ff-", result);
}
