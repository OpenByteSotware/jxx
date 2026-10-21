#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.TimeZone.h"

namespace {
using Calendar = ::jxx::util::GregorianCalendar;
using Date = ::jxx::util::Date;
using Locale = ::jxx::util::Locale;
using String = ::jxx::lang::String;

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
    output->flush();
    const auto serialized = bytes->toByteArray();
    auto byteInput = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(serialized);
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(byteInput);
    return ::jxx::CAST<T>(input->readObject());
}

TEST(GregorianCalendarSerialization, RoundTripsCalendarSpecificState) {
    ::jxx::initialize();
    auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<String>("America/New_York"));
    ASSERT_NE(zone, nullptr);
    ASSERT_EQ("America/New_York", zone->getID()->utf8());
    auto locale = ::jxx::NEW<Locale>(
        ::jxx::NEW<String>("fr"), ::jxx::NEW<String>("CA"));
    auto original = ::jxx::NEW<Calendar>(zone, locale);
    original->setTimeInMillis(1704067200123LL);
    original->setGregorianChange(::jxx::NEW<Date>(-2208988800000LL));
    auto restored = roundTrip(original);
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(original->getTimeInMillis(), restored->getTimeInMillis());
    EXPECT_EQ("America/New_York", restored->getTimeZone()->getID()->utf8());
    EXPECT_EQ(original->getGregorianChange()->getTime(), restored->getGregorianChange()->getTime());
    EXPECT_EQ("gregory", restored->getCalendarType()->utf8());
}

TEST(GregorianCalendarSerialization, RoundTripsTypedReferenceArrays) {
    using Array = ::jxx::lang::JxxArray<::jxx::Ptr<Calendar>, 1U>;
    using Matrix = ::jxx::lang::JxxArray<::jxx::Ptr<Calendar>, 2U>;
    using Row = Matrix::SubArray;
    ::jxx::initialize();
    auto array = ::jxx::NEW<Array>(1);
    (*array)[0] = ::jxx::NEW<Calendar>();
    (*array)[0]->setTimeInMillis(42);
    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    EXPECT_EQ(42, (*restoredArray)[0]->getTimeInMillis());
    auto matrix = ::jxx::NEW<Matrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<Calendar>();
    (*row)[0]->setTimeInMillis(84);
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ(84, (*restoredRow)[0]->getTimeInMillis());
}

TEST(GregorianCalendarSerialization, NoDataRestoresDefaults) {
    auto value = ::jxx::NEW<Calendar>();
    value->setTimeInMillis(123);
    value->setGregorianChange(::jxx::NEW<Date>(456));
    value->readObjectNoData();
    EXPECT_EQ(0, value->getTimeInMillis());
    EXPECT_EQ(-12219292800000LL, value->getGregorianChange()->getTime());
    ASSERT_NE(value->getTimeZone(), nullptr);
    EXPECT_EQ(::jxx::util::TimeZone::getDefault()->getID()->utf8(),
        value->getTimeZone()->getID()->utf8());
}
} // namespace
