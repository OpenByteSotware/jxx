#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.PluralRules.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::util::Locale;
using jxx::util::PluralRules;

TEST(PluralRulesTests, EnglishUsesOneOther) {
    auto rules = PluralRules::forLocale(Locale::US);
    ExpectStringEq(rules->select(1), "one");
    ExpectStringEq(rules->select(2), "other");
}

TEST(PluralRulesTests, FrenchUsesOneForZeroAndOneInThisImplementation) {
    auto rules = PluralRules::forLocale(Locale::FRANCE);
    ExpectStringEq(rules->select(0), "one");
    ExpectStringEq(rules->select(1), "one");
    ExpectStringEq(rules->select(2), "other");
}

TEST(PluralRulesTests, RussianUsesOneFewOtherSubset) {
    auto ru = Locale::forLanguageTag(jxx::lang::String::valueOf("ru-RU"));
    auto rules = PluralRules::forLocale(ru);
    ExpectStringEq(rules->select(1), "one");
    ExpectStringEq(rules->select(2), "few");
    ExpectStringEq(rules->select(5), "other");
}

} // namespace
