#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"

namespace {

using Symbols = ::jxx::text::DateFormatSymbols;
using String = ::jxx::lang::String;

::jxx::Ptr<Symbols> roundTrip(const ::jxx::Ptr<Symbols>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }
    auto inputBytes =
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<Symbols>(input->readObject());
}

TEST(DateFormatSymbolsSerialization, RoundTripsCustomizedSymbols) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Symbols>();

    auto eras = original->getEras();
    (*eras)[0] = ::jxx::NEW<String>("Before");
    (*eras)[1] = ::jxx::NEW<String>("After");
    original->setEras(eras);
    original->setLocalPatternChars(::jxx::NEW<String>("GyMdkHmsSEDFwWahKzZ"));

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(original->equals(restored));
    EXPECT_EQ("Before", (*restored->getEras())[0]->utf8());
    EXPECT_EQ("After", (*restored->getEras())[1]->utf8());
    EXPECT_EQ(
        original->getLocalPatternChars()->utf8(),
        restored->getLocalPatternChars()->utf8());
}

TEST(DateFormatSymbolsSerialization, PreservesZoneStringMatrix) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Symbols>();
    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    auto expected = original->getZoneStrings();
    auto actual = restored->getZoneStrings();
    ASSERT_NE(expected, nullptr);
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ(expected->length, actual->length);
    EXPECT_TRUE(original->equals(restored));
}

TEST(DateFormatSymbolsSerialization, NoDataRestoresDefaults) {
    auto value = ::jxx::NEW<Symbols>();
    auto eras = value->getEras();
    (*eras)[0] = ::jxx::NEW<String>("Changed");
    value->setEras(eras);

    value->readObjectNoData();

    auto defaults = ::jxx::NEW<Symbols>();
    EXPECT_TRUE(defaults->equals(value));
}

} // namespace
