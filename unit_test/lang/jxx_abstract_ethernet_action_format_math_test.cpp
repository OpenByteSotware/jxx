#include <gtest/gtest.h>

#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Math.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

TEST(AbstractEthernetActionParity, PercentHUsesIntegerHashInHex) {
    using ObjectArray = ::jxx::JxxArray<
        ::jxx::Ptr<::jxx::lang::Object>, 1U>;
    auto arguments = ::jxx::NEW<ObjectArray>(4);
    (*arguments)(0) = ::jxx::NEW<::jxx::lang::Integer>(0x01);
    (*arguments)(1) = ::jxx::NEW<::jxx::lang::Integer>(0x7F);
    (*arguments)(2) = ::jxx::NEW<::jxx::lang::Integer>(0x80);
    (*arguments)(3) = ::jxx::NEW<::jxx::lang::Integer>(0xFF);

    const auto result = ::jxx::lang::String::format(
        ::jxx::NEW<::jxx::lang::String>("%h %h %h %h"),
        arguments);
    EXPECT_EQ("1 7f 80 ff", result->utf8());
}

TEST(AbstractEthernetActionParity, IntegerToHexStringMatchesMessageLogging) {
    EXPECT_EQ("1210", ::jxx::lang::Integer::toHexString(0x1210)->utf8());
    EXPECT_EQ("ffffffff", ::jxx::lang::Integer::toHexString(-1)->utf8());
}

TEST(AbstractEthernetActionParity, MathRandomIsWithinRequiredHalfOpenRange) {
    for (int index = 0; index < 100; ++index) {
        const auto value = ::jxx::lang::Math::random();
        EXPECT_GE(value, 0.0);
        EXPECT_LT(value, 1.0);
    }
}
