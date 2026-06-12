#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.LocaleBuilder.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::lang::String;
using jxx::util::LocaleBuilder;

TEST(LocaleBuilderTests, BuildProducesNormalizedLocale) {
    auto loc = jxx::NEW<LocaleBuilder>()
                   ->setLanguage(String::valueOf("EN"))
                   ->setScript(String::valueOf("hans"))
                   ->setRegion(String::valueOf("cn"))
                   ->setVariant(String::valueOf("PINYIN"))
                   ->build();

    ExpectStringEq(loc->getLanguage(), "en");
    ExpectStringEq(loc->getScript(), "Hans");
    ExpectStringEq(loc->getCountry(), "CN");
    ExpectStringEq(loc->getVariant(), "PINYIN");
}

TEST(LocaleBuilderTests, ClearResetsState) {
    auto builder = jxx::NEW<LocaleBuilder>();
    builder->setLanguage(String::valueOf("fr"))
           ->setRegion(String::valueOf("FR"))
           ->clear();

    auto loc = builder->build();
    ExpectStringEq(loc->getLanguage(), "");
    ExpectStringEq(loc->getCountry(), "");
    ExpectStringEq(loc->toString(), "");
}

} // namespace
