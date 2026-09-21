#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::Integer;
using ::jxx::lang::Long;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::String;
using ::jxx::lang::System;

TEST(SystemPropertyParityTest, RejectsNullAndEmptyKeysWithCorrectExceptions) {
    const ::jxx::Ptr<String> nullString;
    EXPECT_THROW(System::getProperty(nullString), NullPointerException);
    EXPECT_THROW(System::setProperty(nullString, ::jxx::NEW<String>("value")),
                 NullPointerException);
    EXPECT_THROW(System::clearProperty(nullString), NullPointerException);

    const auto empty = ::jxx::NEW<String>("");
    EXPECT_THROW(System::getProperty(empty), IllegalArgumentException);
    EXPECT_THROW(System::setProperty(empty, ::jxx::NEW<String>("value")),
                 IllegalArgumentException);
    EXPECT_THROW(System::clearProperty(empty), IllegalArgumentException);
}

TEST(SystemPropertyParityTest, SetAndClearReturnPreviousValues) {
    const auto key = ::jxx::NEW<String>("jxx.test.system.property.lifecycle");
    System::clearProperty(key);

    EXPECT_EQ(nullptr, System::setProperty(key, ::jxx::NEW<String>("first")));
    EXPECT_EQ("first", System::getProperty(key)->utf8());
    EXPECT_EQ("first", System::setProperty(key, ::jxx::NEW<String>("second"))->utf8());
    EXPECT_EQ("second", System::clearProperty(key)->utf8());
    EXPECT_EQ(nullptr, System::getProperty(key));
    EXPECT_EQ(nullptr, System::clearProperty(key));
}

TEST(SystemPropertyParityTest, DefaultIsReturnedOnlyWhenPropertyIsAbsent) {
    const auto key = ::jxx::NEW<String>("jxx.test.system.property.default");
    const auto fallback = ::jxx::NEW<String>("fallback");
    System::clearProperty(key);

    EXPECT_EQ(fallback.get(), System::getProperty(key, fallback).get());
    System::setProperty(key, ::jxx::NEW<String>(""));
    EXPECT_EQ("", System::getProperty(key, fallback)->utf8());
    System::clearProperty(key);
}

TEST(SystemPropertyParityTest, NullValuesAreRejectedWithoutChangingProperty) {
    const auto key = ::jxx::NEW<String>("jxx.test.system.property.null-value");
    System::setProperty(key, ::jxx::NEW<String>("retained"));
    const ::jxx::Ptr<String> nullString;

    EXPECT_THROW(System::setProperty(key, nullString), NullPointerException);
    EXPECT_EQ("retained", System::getProperty(key)->utf8());
    System::clearProperty(key);
}

TEST(SystemPropertyParityTest, PropertiesRemainSeparateFromEnvironmentVariables) {
    const auto environmentName = ::jxx::NEW<String>("PATH");
    const auto environmentValue = System::getenv(environmentName);
    System::clearProperty(environmentName);

    EXPECT_EQ(nullptr, System::getProperty(environmentName));
    if (environmentValue != nullptr) {
        EXPECT_EQ(environmentValue->utf8(), System::getenv(environmentName)->utf8());
    }
}

TEST(SystemPropertyParityTest, WrapperPropertyLookupsObserveSystemProperties) {
    const auto integerKey = ::jxx::NEW<String>("jxx.test.system.integer");
    const auto longKey = ::jxx::NEW<String>("jxx.test.system.long");
    System::setProperty(integerKey, ::jxx::NEW<String>("0x2a"));
    System::setProperty(longKey, ::jxx::NEW<String>("#2a"));

    EXPECT_EQ(42, Integer::getInteger(integerKey)->intValue());
    EXPECT_EQ(42LL, Long::getLong(longKey)->longValue());

    System::clearProperty(integerKey);
    System::clearProperty(longKey);
}

} // namespace
