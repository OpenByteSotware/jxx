#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Currency.h"
#include "util/jxx.util.Locale.h"
#include "tests/TestHelpers.h"

namespace {
using namespace jxx::tests;
using jxx::lang::String;
using jxx::util::Currency;
using jxx::util::Locale;

TEST(CurrencyTests, GetInstanceByCodeWorks) {
    auto usd = Currency::getInstance(String::valueOf("USD"));
    ASSERT_TRUE(usd != nullptr);
    ExpectStringEq(usd->getCurrencyCode(), "USD");
    ExpectStringEq(usd->getSymbol(), "$");
}

TEST(CurrencyTests, GetInstanceByLocaleWorks) {
    auto eur = Currency::getInstance(Locale::FRANCE);
    ASSERT_TRUE(eur != nullptr);
    ExpectStringEq(eur->getCurrencyCode(), "EUR");
}

TEST(CurrencyTests, CachesByCurrencyCode) {
    auto a = Currency::getInstance(String::valueOf("USD"));
    auto b = Currency::getInstance(String::valueOf("USD"));
    ASSERT_TRUE(a != nullptr);
    ASSERT_TRUE(b != nullptr);
    EXPECT_EQ(a.get(), b.get());
}

} // namespace
