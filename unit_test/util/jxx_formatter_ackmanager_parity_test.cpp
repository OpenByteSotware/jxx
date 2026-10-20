#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

using FormatArguments = ::jxx::JxxArray<
    ::jxx::Ptr<::jxx::lang::Object>, 1U>;

}

TEST(FormatterAckManagerParity, FormatsHashAndStringConversions)
{
    ::jxx::initialize();
    auto arguments = ::jxx::NEW<FormatArguments>(6);
    (*arguments)(0) = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::valueOf(1));
    (*arguments)(1) = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::valueOf(2));
    (*arguments)(2) = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::valueOf(3));
    (*arguments)(3) = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::valueOf(4));
    (*arguments)(4) = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::toHexString(0x1102));
    (*arguments)(5) = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<::jxx::lang::String>("192.0.2.10"));

    auto result = ::jxx::lang::String::format(
        ::jxx::NEW<::jxx::lang::String>(
            "No reply to packet ID %h %h %h %h, message ID %s (%s)"),
        arguments);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->utf8(),
        "No reply to packet ID 1 2 3 4, message ID 1102 (192.0.2.10)");
}

TEST(FormatterAckManagerParity, NullHashAndStringMatchSourceSemantics)
{
    ::jxx::initialize();
    auto arguments = ::jxx::NEW<FormatArguments>(2);
    (*arguments)(0) = nullptr;
    (*arguments)(1) = nullptr;
    auto result = ::jxx::lang::String::format(
        ::jxx::NEW<::jxx::lang::String>("%h %s"), arguments);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->utf8(), "null null");
}
