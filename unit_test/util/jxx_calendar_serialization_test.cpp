#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.TimeZone.h"

namespace {
using Calendar = ::jxx::util::Calendar;
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

TEST(CalendarSerialization, RoundTripsInstantAndTimeZoneIdentity) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Calendar>();
    original->setTimeInMillis(1704067200123LL);
    original->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<String>("America/New_York")));

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(original->getTimeInMillis(), restored->getTimeInMillis());
    ASSERT_NE(restored->getTimeZone(), nullptr);
    EXPECT_EQ("America/New_York", restored->getTimeZone()->getID()->utf8());
    EXPECT_EQ(original->get(Calendar::YEAR), restored->get(Calendar::YEAR));
    EXPECT_EQ(original->get(Calendar::MONTH), restored->get(Calendar::MONTH));
    EXPECT_EQ(original->get(Calendar::DAY_OF_MONTH),
        restored->get(Calendar::DAY_OF_MONTH));
}

TEST(CalendarSerialization, RoundTripsTypedReferenceArrays) {
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
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ(84, (*restoredRow)[0]->getTimeInMillis());
}

TEST(CalendarSerialization, NoDataRestoresEpochAndDefaultZone) {
    auto value = ::jxx::NEW<Calendar>();
    value->setTimeInMillis(12345);
    value->readObjectNoData();
    EXPECT_EQ(0, value->getTimeInMillis());
    ASSERT_NE(value->getTimeZone(), nullptr);
    ASSERT_NE(::jxx::util::TimeZone::getDefault(), nullptr);
    EXPECT_EQ(::jxx::util::TimeZone::getDefault()->getID()->utf8(),
        value->getTimeZone()->getID()->utf8());
}
} // namespace
