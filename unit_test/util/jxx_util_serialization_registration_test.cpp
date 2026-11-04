#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/jxx.util.BitSet.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.Currency.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.EventObject.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.Random.h"
#include "util/jxx.util.UUID.h"

namespace {

template <typename T>
void expectSerialVersionUid(const ::jxx::lang::jlong expected)
{
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            T::Class()->getName()->utf8());
    EXPECT_EQ(expected, metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}

TEST(JxxUtilSerializationRegistrationTest, UsesJava8SerialVersionUids)
{
    ::jxx::initialize();

    expectSerialVersionUid<::jxx::util::BitSet>(
        static_cast<::jxx::lang::jlong>(7997698588986878753LL));
    expectSerialVersionUid<::jxx::util::Calendar>(
        static_cast<::jxx::lang::jlong>(-1807547505821590642LL));
    expectSerialVersionUid<::jxx::util::Date>(
        static_cast<::jxx::lang::jlong>(7523967970034938905LL));
    expectSerialVersionUid<::jxx::util::GregorianCalendar>(
        static_cast<::jxx::lang::jlong>(-8125100834729963327LL));
    expectSerialVersionUid<::jxx::util::Currency>(
        static_cast<::jxx::lang::jlong>(-158308464356906721LL));
    expectSerialVersionUid<::jxx::util::Locale>(
        static_cast<::jxx::lang::jlong>(9149081749638150636LL));
    expectSerialVersionUid<::jxx::util::EventObject>(
        static_cast<::jxx::lang::jlong>(5516075349620653480LL));
    expectSerialVersionUid<::jxx::util::Random>(
        static_cast<::jxx::lang::jlong>(3905348978240129619LL));
    expectSerialVersionUid<::jxx::util::UUID>(
        static_cast<::jxx::lang::jlong>(-4856846361193249489LL));
}

} // namespace
