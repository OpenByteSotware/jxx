#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"

TEST(SimpleDateFormatLocalizedPatternParity, TranslatesPatternLettersAndPreservesQuotes) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    symbols->setLocalPatternChars(
        ::jxx::NEW<::jxx::lang::String>("AyMdkHmsSEDFwWahKzZYuXL"));
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy 'G' MM"), symbols);

    EXPECT_TRUE(format->toLocalizedPattern()->equals(
        ::jxx::NEW<::jxx::lang::String>("yyyy 'G' MM")));

    format->applyLocalizedPattern(
        ::jxx::NEW<::jxx::lang::String>("AAAA 'A' MM"));
    EXPECT_TRUE(format->toPattern()->equals(
        ::jxx::NEW<::jxx::lang::String>("GGGG 'A' MM")));
}

TEST(SimpleDateFormatLocalizedPatternParity, RejectsInvalidPatternCharacterMap) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    symbols->setLocalPatternChars(::jxx::NEW<::jxx::lang::String>("short"));
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy"), symbols);
    EXPECT_THROW(format->toLocalizedPattern(), ::jxx::lang::IllegalArgumentException);
}
