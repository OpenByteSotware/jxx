#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"

TEST(CalendarConfigurationSerializationParity, RoundTripsConfigurationState)
{
    const auto source = ::jxx::util::Calendar::getInstance();
    ASSERT_NE(::jxx::CAST<::jxx::util::GregorianCalendar>(source), nullptr);
    EXPECT_EQ(source->getClass(), ::jxx::util::GregorianCalendar::Class());

    source->setTimeInMillis(123456789);
    source->setLenient(false);
    source->setFirstDayOfWeek(::jxx::util::Calendar::MONDAY);
    source->setMinimalDaysInFirstWeek(4);

    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(source));
    output->close();

    const auto inputBytes =
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    const auto restored =
        ::jxx::CAST<::jxx::util::Calendar>(input->readObject());

    ASSERT_NE(restored, nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::util::GregorianCalendar>(restored), nullptr);
    EXPECT_EQ(restored->getClass(), ::jxx::util::GregorianCalendar::Class());
    EXPECT_EQ(restored->getTimeInMillis(), 123456789);
    EXPECT_FALSE(restored->isLenient());
    EXPECT_EQ(restored->getFirstDayOfWeek(), ::jxx::util::Calendar::MONDAY);
    EXPECT_EQ(restored->getMinimalDaysInFirstWeek(), 4);
    EXPECT_TRUE(source->equals(::jxx::CAST<::jxx::lang::Object>(restored)));
    EXPECT_EQ(source->hashCode(), restored->hashCode());
}
