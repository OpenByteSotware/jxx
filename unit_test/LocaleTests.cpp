#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Locale.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::lang::String;
using jxx::util::Locale;

TEST(LocaleTests, PredefinedConstantsExposeExpectedValues) {
    ExpectStringEq(Locale::US->getLanguage(), "en");
    ExpectStringEq(Locale::US->getCountry(), "US");
    ExpectStringEq(Locale::FRANCE->getLanguage(), "fr");
    ExpectStringEq(Locale::FRANCE->getCountry(), "FR");
}

TEST(LocaleTests, ForLanguageTagParsesLanguageAndCountry) {
    auto loc = Locale::forLanguageTag(String::valueOf("en-US"));
    ExpectStringEq(loc->getLanguage(), "en");
    ExpectStringEq(loc->getCountry(), "US");
    ExpectStringEq(loc->toString(), "en_US");
    ExpectStringEq(loc->toLanguageTag(), "en-US");
}

TEST(LocaleTests, ForLanguageTagParsesScriptAndRegion) {
    auto loc = Locale::forLanguageTag(String::valueOf("zh-Hans-CN"));
    ExpectStringEq(loc->getLanguage(), "zh");
    ExpectStringEq(loc->getScript(), "Hans");
    ExpectStringEq(loc->getCountry(), "CN");
    ExpectStringEq(loc->toLanguageTag(), "zh-Hans-CN");
}

TEST(LocaleTests, DisplayAndIsoDataUseTinyEmbeddedTables) {
    auto loc = Locale::forLanguageTag(String::valueOf("en-US"));
    ExpectStringEq(loc->getDisplayLanguage(), "English");
    ExpectStringEq(loc->getDisplayCountry(), "United States");
    ExpectStringEq(loc->getISO3Language(), "eng");
    ExpectStringEq(loc->getISO3Country(), "USA");
}

TEST(LocaleTests, InternReturnsCanonicalSharedInstance) {
    auto a = Locale::intern(String::valueOf("en"), String::valueOf("US"), String::valueOf(""));
    auto b = Locale::intern(String::valueOf("en"), String::valueOf("US"), String::valueOf(""));
    ASSERT_TRUE(a != nullptr);
    ASSERT_TRUE(b != nullptr);
    EXPECT_EQ(a.get(), b.get());
}

TEST(LocaleTests, EqualityAndHashMatchNormalizedValues) {
    auto a = Locale::forLanguageTag(String::valueOf("en-US"));
    auto b = Locale::intern(String::valueOf("EN"), String::valueOf("US"), String::valueOf(""));
    EXPECT_TRUE(a->equals(b));
    EXPECT_EQ(a->hashCode(), b->hashCode());
}

} // namespace
