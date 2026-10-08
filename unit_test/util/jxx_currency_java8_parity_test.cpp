#include <gtest/gtest.h>
#include <type_traits>

#include "util/jxx.util.Currency.h"
#include "util/jxx.util.StringPool.h"

TEST(JxxCurrencyJava8ParityTest, IsFinalAndSerializable) {
    static_assert(std::is_final_v<::jxx::util::Currency>);
    EXPECT_TRUE(::jxx::io::SerializableI::Class()->isAssignableFrom(
        ::jxx::util::Currency::Class()));
}

TEST(JxxCurrencyJava8ParityTest, InstancesAreCanonicalByCode) {
    const auto code = ::jxx::util::StringPool::intern("USD");
    const auto first = ::jxx::util::Currency::getInstance(code);
    const auto second = ::jxx::util::Currency::getInstance(code);
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first, second);
    EXPECT_EQ(first->getNumericCode(), 840);
    EXPECT_EQ(first->getDefaultFractionDigits(), 2);
}

TEST(JxxCurrencyJava8ParityTest, ZeroFractionCurrencyMetadataIsPreserved) {
    const auto currency = ::jxx::util::Currency::getInstance(
        ::jxx::util::StringPool::intern("JPY"));
    ASSERT_NE(currency, nullptr);
    EXPECT_EQ(currency->getNumericCode(), 392);
    EXPECT_EQ(currency->getDefaultFractionDigits(), 0);
}
