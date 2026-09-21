#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Properties.h"

namespace {
using ::jxx::lang::String;
using ::jxx::lang::System;
using ::jxx::util::Properties;

TEST(SystemPropertiesObjectParityTest, GetPropertiesReturnsLiveObject) {
    const auto original = System::getProperties();
    const auto key = ::jxx::NEW<String>("jxx.test.live-properties");
    original->setProperty(key, ::jxx::NEW<String>("value"));
    EXPECT_EQ("value", System::getProperty(key)->utf8());
    System::clearProperty(key);
}

TEST(SystemPropertiesObjectParityTest, SetPropertiesReplacesCurrentObject) {
    const auto original = System::getProperties();
    const auto replacement = ::jxx::NEW<Properties>();
    const auto key = ::jxx::NEW<String>("jxx.test.replacement-properties");
    replacement->setProperty(key, ::jxx::NEW<String>("replacement"));

    System::setProperties(replacement);
    EXPECT_EQ(replacement.get(), System::getProperties().get());
    EXPECT_EQ("replacement", System::getProperty(key)->utf8());

    System::setProperties(original);
}

TEST(SystemPropertiesObjectParityTest, NullSetPropertiesCreatesNewEmptyObject) {
    const auto original = System::getProperties();
    const auto key = ::jxx::NEW<String>("jxx.test.cleared-properties");
    System::setProperty(key, ::jxx::NEW<String>("old"));

    const ::jxx::Ptr<Properties> none;
    System::setProperties(none);
    ASSERT_NE(nullptr, System::getProperties());
    EXPECT_EQ(nullptr, System::getProperty(key));

    System::setProperties(original);
    System::clearProperty(key);
}

} // namespace
