#include <gtest/gtest.h>

#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicBoolean.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicInteger.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLong.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicMarkableReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicStampedReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAdder.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAccumulator.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReferenceArray.h"

namespace atomic = ::jxx::util::concurrent::atomic;

namespace {
template <typename T>
void expectSerialVersionUID(::jxx::lang::jlong expected) {
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            T::Class()->getName()->utf8());
    EXPECT_EQ(expected, metadata.serialVersionUID);
    EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
}
}

TEST(AtomicPackageInitializationParity, RegistersAllImplementedPublicTypeMetadata) {
    atomic::initialize();
    EXPECT_NE(nullptr, atomic::AtomicBoolean::Class());
    EXPECT_NE(nullptr, atomic::AtomicInteger::Class());
    EXPECT_NE(nullptr, atomic::AtomicLong::Class());
    EXPECT_NE(nullptr, atomic::AtomicReference<::jxx::lang::Object>::Class());
    EXPECT_NE(nullptr, atomic::AtomicReferenceArray<::jxx::lang::Object>::Class());
    EXPECT_NE(nullptr, atomic::AtomicMarkableReference<::jxx::lang::Object>::Class());
    EXPECT_NE(nullptr, atomic::AtomicStampedReference<::jxx::lang::Object>::Class());
}

TEST(AtomicPackageInitializationParity, RegistersSourceSerialVersionUIDs) {
    atomic::initialize();
    expectSerialVersionUID<atomic::AtomicBoolean>(4654671469794556979LL);
    expectSerialVersionUID<atomic::AtomicInteger>(6214790243416807050LL);
    expectSerialVersionUID<atomic::AtomicIntegerArray>(2862133569453604235LL);
    expectSerialVersionUID<atomic::AtomicLong>(1927816293512124184LL);
    expectSerialVersionUID<atomic::AtomicLongArray>(-2308431214976778248LL);
    expectSerialVersionUID<atomic::AtomicReference<::jxx::lang::Object>>(-1848883965231344442LL);
    expectSerialVersionUID<atomic::AtomicReferenceArray<::jxx::lang::Object>>(-6209656149925076980LL);
    expectSerialVersionUID<atomic::DoubleAccumulator>(7249069246863182397LL);
    expectSerialVersionUID<atomic::DoubleAdder>(7249069246863182397LL);
    expectSerialVersionUID<atomic::LongAccumulator>(7249069246863182397LL);
    expectSerialVersionUID<atomic::LongAdder>(7249069246863182397LL);
}
