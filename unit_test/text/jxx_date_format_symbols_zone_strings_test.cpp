#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

TEST(DateFormatSymbolsZoneStringsParity, GetterReturnsDeepCopy) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto first = symbols->getZoneStrings();
    ASSERT_NE(nullptr, first);
    ASSERT_GT(first->length, 0);
    (*first)[0][1] = ::jxx::NEW<::jxx::lang::String>("Changed");
    auto second = symbols->getZoneStrings();
    EXPECT_FALSE((*second)[0][1]->equals((*first)[0][1]));
}

TEST(DateFormatSymbolsZoneStringsParity, SetterDeepCopiesRows) {
    using Symbols = ::jxx::text::DateFormatSymbols;
    using Row = Symbols::ZoneStringArray::SubArray;
    std::vector<::jxx::Ptr<Row>> rows;
    rows.push_back(::jxx::NEW<Row>(std::initializer_list<::jxx::Ptr<::jxx::lang::String>>{
        ::jxx::NEW<::jxx::lang::String>("Test/Zone"),
        ::jxx::NEW<::jxx::lang::String>("Test Standard Time"),
        ::jxx::NEW<::jxx::lang::String>("TST"),
        ::jxx::NEW<::jxx::lang::String>("Test Daylight Time"),
        ::jxx::NEW<::jxx::lang::String>("TDT")}));
    auto values = ::jxx::NEW<Symbols::ZoneStringArray>(std::move(rows));
    auto symbols = ::jxx::NEW<Symbols>();
    symbols->setZoneStrings(values);
    (*values)[0][1] = ::jxx::NEW<::jxx::lang::String>("Changed");
    EXPECT_EQ("Test Standard Time", (*symbols->getZoneStrings())[0][1]->utf8());
}

TEST(DateFormatSymbolsZoneStringsParity, RejectsRowsWithFewerThanFiveColumns) {
    using Symbols = ::jxx::text::DateFormatSymbols;
    using Row = Symbols::ZoneStringArray::SubArray;
    std::vector<::jxx::Ptr<Row>> rows;
    rows.push_back(::jxx::NEW<Row>(4));
    auto values = ::jxx::NEW<Symbols::ZoneStringArray>(std::move(rows));
    auto symbols = ::jxx::NEW<Symbols>();
    EXPECT_THROW(symbols->setZoneStrings(values), ::jxx::lang::IllegalArgumentException);
}
