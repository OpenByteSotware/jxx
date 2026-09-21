#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {
using ::jxx::lang::Integer;
using ::jxx::lang::Long;
using ::jxx::lang::Short;
using ::jxx::lang::String;
using ::jxx::lang::System;

TEST(ShortJava8ParityTest, ConvertsAllBitsToUnsignedWidenedValues) {
    EXPECT_EQ(0, Short::toUnsignedInt(0));
    EXPECT_EQ(65535, Short::toUnsignedInt(static_cast<::jxx::lang::jshort>(-1)));
    EXPECT_EQ(65535LL, Short::toUnsignedLong(static_cast<::jxx::lang::jshort>(-1)));
    EXPECT_EQ(32768, Short::toUnsignedInt(Short::MIN_VALUE));
}

TEST(IntegerPropertyParityTest, DecodesPresentPropertyAndUsesDefaults) {
    const auto key = ::jxx::NEW<String>("jxx.test.integer.property");
    System::setProperty(key, ::jxx::NEW<String>("0x2a"));
    ASSERT_NE(nullptr, Integer::getInteger(key));
    EXPECT_EQ(42, Integer::getInteger(key)->intValue());

    System::setProperty(key, ::jxx::NEW<String>("not-a-number"));
    EXPECT_EQ(7, Integer::getInteger(key, 7)->intValue());
    const auto fallback = ::jxx::NEW<Integer>(9);
    EXPECT_EQ(fallback.get(), Integer::getInteger(key, fallback).get());

    System::clearProperty(key);
    EXPECT_EQ(nullptr, Integer::getInteger(key));
    EXPECT_EQ(11, Integer::getInteger(key, 11)->intValue());
}

TEST(IntegerPropertyParityTest, NullAndEmptyNamesReturnDefaults) {
    const auto fallback = ::jxx::NEW<Integer>(17);
    EXPECT_EQ(nullptr, Integer::getInteger(nullptr));
    EXPECT_EQ(fallback.get(), Integer::getInteger(nullptr, fallback).get());
    EXPECT_EQ(fallback.get(), Integer::getInteger(::jxx::NEW<String>(""), fallback).get());
}

TEST(LongPropertyParityTest, DecodesPresentPropertyAndUsesDefaults) {
    const auto key = ::jxx::NEW<String>("jxx.test.long.property");
    System::setProperty(key, ::jxx::NEW<String>("#2a"));
    ASSERT_NE(nullptr, Long::getLong(key));
    EXPECT_EQ(42LL, Long::getLong(key)->longValue());

    System::setProperty(key, ::jxx::NEW<String>("invalid"));
    EXPECT_EQ(7LL, Long::getLong(key, static_cast<::jxx::lang::jlong>(7))->longValue());
    const auto fallback = ::jxx::NEW<Long>(static_cast<::jxx::lang::jlong>(9));
    EXPECT_EQ(fallback.get(), Long::getLong(key, fallback).get());

    System::clearProperty(key);
    EXPECT_EQ(nullptr, Long::getLong(key));
}

TEST(LongPropertyParityTest, NullAndEmptyNamesReturnDefaults) {
    const auto fallback = ::jxx::NEW<Long>(static_cast<::jxx::lang::jlong>(17));
    EXPECT_EQ(nullptr, Long::getLong(nullptr));
    EXPECT_EQ(fallback.get(), Long::getLong(nullptr, fallback).get());
    EXPECT_EQ(fallback.get(), Long::getLong(::jxx::NEW<String>(""), fallback).get());
}

} // namespace
