#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassLoader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.ResourceBundle.h"

TEST(LocaleParity, ProvidesJava8ConstantsAndDefaultMutation) {
    ASSERT_NE(nullptr, ::jxx::util::Locale::ENGLISH);
    ASSERT_NE(nullptr, ::jxx::util::Locale::ITALY);
    EXPECT_EQ("en", ::jxx::util::Locale::ENGLISH->getLanguage()->utf8());
    EXPECT_EQ("IT", ::jxx::util::Locale::ITALY->getCountry()->utf8());

    const auto previous = ::jxx::util::Locale::getDefault();
    ::jxx::util::Locale::setDefault(::jxx::util::Locale::FRANCE);
    EXPECT_EQ(::jxx::util::Locale::FRANCE.get(),
              ::jxx::util::Locale::getDefault().get());
    ::jxx::util::Locale::setDefault(previous);
}

TEST(LocaleParity, ReturnsDefensiveAvailableAndIsoArrays) {
    const auto first = ::jxx::util::Locale::getAvailableLocales();
    const auto second = ::jxx::util::Locale::getAvailableLocales();
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_NE(first.get(), second.get());
    EXPECT_GT(first->length, 0);
    EXPECT_GT(::jxx::util::Locale::getISOCountries()->length, 0);
    EXPECT_GT(::jxx::util::Locale::getISOLanguages()->length, 0);
}

TEST(LocaleParity, LocaleSensitiveDisplayOverloadsRejectNull) {
    EXPECT_THROW(
        ::jxx::util::Locale::US->getDisplayName(nullptr),
        ::jxx::lang::NullPointerException);
}

TEST(ResourceBundleParity, LoaderSpecificOperationsRejectNull) {
    const auto baseName = ::jxx::NEW<::jxx::lang::String>("example.Bundle");
    EXPECT_THROW(
        ::jxx::util::ResourceBundle::getBundle(
            baseName, ::jxx::util::Locale::US, nullptr),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::util::ResourceBundle::clearCache(nullptr),
        ::jxx::lang::NullPointerException);
}
