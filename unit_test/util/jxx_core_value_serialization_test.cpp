#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.UUID.h"

namespace {
template <typename T>
jxx::Ptr<T> roundTrip(const jxx::Ptr<T>& value) {
    auto bytes = jxx::NEW<jxx::io::ByteArrayOutputStream>();
    auto output = jxx::NEW<jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value); output->flush();
    auto input = jxx::NEW<jxx::io::ObjectInputStream>(
        jxx::NEW<jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    return jxx::CAST<T>(input->readObject());
}
}

TEST(JxxCoreValueSerialization, RoundTripsDateMillis) {
    jxx::initialize();
    auto restored = roundTrip(jxx::NEW<jxx::util::Date>(-1234567890123LL));
    EXPECT_EQ(-1234567890123LL, restored->getTime());
}

TEST(JxxCoreValueSerialization, RoundTripsLocaleComponents) {
    jxx::initialize();
    auto value = jxx::NEW<jxx::util::Locale>(
        jxx::NEW<jxx::lang::String>("en"),
        jxx::NEW<jxx::lang::String>("Latn"),
        jxx::NEW<jxx::lang::String>("US"),
        jxx::NEW<jxx::lang::String>("POSIX"));
    auto restored = roundTrip(value);
    EXPECT_EQ("en", restored->getLanguage()->utf8());
    EXPECT_EQ("Latn", restored->getScript()->utf8());
    EXPECT_EQ("US", restored->getCountry()->utf8());
    EXPECT_EQ("POSIX", restored->getVariant()->utf8());
}

TEST(JxxCoreValueSerialization, RoundTripsUuidBits) {
    jxx::initialize();
    auto value = jxx::NEW<jxx::util::UUID>(
        static_cast<jxx::lang::jlong>(0x123456789abcdef0ULL),
        static_cast<jxx::lang::jlong>(0xfedcba9876543210ULL));
    auto restored = roundTrip(value);
    EXPECT_EQ(value->getMostSignificantBits(), restored->getMostSignificantBits());
    EXPECT_EQ(value->getLeastSignificantBits(), restored->getLeastSignificantBits());
}
