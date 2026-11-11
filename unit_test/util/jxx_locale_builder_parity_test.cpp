#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.IllformedLocaleException.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.LocaleBuilder.h"

TEST(LocaleBuilderParity, ExposesNestedJava8TypeName) {
    EXPECT_TRUE((std::is_same_v<
        ::jxx::util::Locale::Builder,
        ::jxx::util::LocaleBuilder>));
}

TEST(LocaleBuilderParity, BuildsNormalizedLocale) {
    const auto builder = ::jxx::NEW<::jxx::util::Locale::Builder>();
    const auto locale = builder
        ->setLanguage(::jxx::NEW<::jxx::lang::String>("EN"))
        ->setScript(::jxx::NEW<::jxx::lang::String>("latn"))
        ->setRegion(::jxx::NEW<::jxx::lang::String>("us"))
        ->setVariant(::jxx::NEW<::jxx::lang::String>("POSIX"))
        ->build();

    EXPECT_EQ("en", locale->getLanguage()->utf8());
    EXPECT_EQ("Latn", locale->getScript()->utf8());
    EXPECT_EQ("US", locale->getCountry()->utf8());
    EXPECT_EQ("POSIX", locale->getVariant()->utf8());
}

TEST(LocaleBuilderParity, RejectsNullAndIllformedFields) {
    const auto builder = ::jxx::NEW<::jxx::util::Locale::Builder>();
    EXPECT_THROW(builder->setLanguage(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        builder->setLanguage(::jxx::NEW<::jxx::lang::String>("e")),
        ::jxx::util::IllformedLocaleException);
    EXPECT_THROW(
        builder->setScript(::jxx::NEW<::jxx::lang::String>("latin")),
        ::jxx::util::IllformedLocaleException);
    EXPECT_THROW(
        builder->setRegion(::jxx::NEW<::jxx::lang::String>("USA1")),
        ::jxx::util::IllformedLocaleException);
}

TEST(LocaleBuilderParity, EmptyValuesClearIndividualFields) {
    const auto empty = ::jxx::NEW<::jxx::lang::String>("");
    const auto builder = ::jxx::NEW<::jxx::util::Locale::Builder>();
    const auto locale = builder
        ->setLanguage(::jxx::NEW<::jxx::lang::String>("en"))
        ->setRegion(::jxx::NEW<::jxx::lang::String>("US"))
        ->setLanguage(empty)
        ->setRegion(empty)
        ->build();
    EXPECT_TRUE(locale->getLanguage()->isEmpty());
    EXPECT_TRUE(locale->getCountry()->isEmpty());
}
