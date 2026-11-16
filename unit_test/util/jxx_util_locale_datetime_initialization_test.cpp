#include <gtest/gtest.h>

#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ClassInitialization.h"
#include "util/jxx.util.Currency.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.SimpleTimeZone.h"
#include "util/jxx.util.TimeZone.h"

namespace {
template <typename T>
void expectSerializableRegistration(::jxx::lang::jlong expectedUid) {
    const auto clazz = T::Class();
    ASSERT_NE(nullptr, clazz);
    ASSERT_NE(nullptr, clazz->getName());
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            clazz->getName()->utf8());
    EXPECT_EQ(expectedUid, metadata.serialVersionUID);
    ASSERT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    EXPECT_NE(nullptr, metadata.deserializationFactory());
}
} // namespace

TEST(UtilLocaleDateTimeInitialization, RegistersSerializableParityTypes) {
    ::jxx::util::initializeClasses();

    expectSerializableRegistration<::jxx::util::Locale>(
        static_cast<::jxx::lang::jlong>(9149081749638150636LL));
    expectSerializableRegistration<::jxx::util::Currency>(
        static_cast<::jxx::lang::jlong>(-158308464356906721LL));
    expectSerializableRegistration<::jxx::util::Date>(
        static_cast<::jxx::lang::jlong>(7523967970034938905LL));
    expectSerializableRegistration<::jxx::util::GregorianCalendar>(
        static_cast<::jxx::lang::jlong>(-8125100834729963327LL));
    expectSerializableRegistration<::jxx::util::SimpleTimeZone>(
        static_cast<::jxx::lang::jlong>(-403250971215465050LL));

    EXPECT_NE(nullptr, ::jxx::util::TimeZone::Class());
}

TEST(UtilSimpleTimeZoneSerializationState, FactoryCreatesSourceCompatibleDefault) {
    ::jxx::util::initializeClasses();
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            ::jxx::util::SimpleTimeZone::Class()->getName()->utf8());
    const auto restored = ::jxx::CAST<::jxx::util::SimpleTimeZone>(
        metadata.deserializationFactory());
    ASSERT_NE(nullptr, restored);
    ASSERT_NE(nullptr, restored->getID());
    EXPECT_EQ("GMT", restored->getID()->utf8());
    EXPECT_EQ(0, restored->getRawOffset());
}
