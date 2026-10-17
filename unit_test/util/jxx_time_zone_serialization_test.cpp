#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.TimeZone.h"

TEST(TimeZoneSerialization, ReconstructsZoneFromIdentifier) {
    ::jxx::initialize();
    const auto source = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT+05:30"));
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); output->writeObject(source); output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    const auto first = std::dynamic_pointer_cast<::jxx::util::TimeZone>(input->readObject());
    const auto second = input->readObject();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first->getID()->utf8(), source->getID()->utf8());
    EXPECT_EQ(first->getRawOffset(), source->getRawOffset());
    EXPECT_EQ(first.get(), second.get());
}
