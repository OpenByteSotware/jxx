#include <gtest/gtest.h>
#include "jxx.initialize.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "util/jxx.util.IntStream.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicIntegerArray.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicLongArray.h"

namespace {
template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(value);
    output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    return ::jxx::CAST<T>(input->readObject());
}
}

TEST(JxxSerializableUtilityBatch, RoundTripsAtomicIntegerArray) {
    ::jxx::initialize();
    auto value = ::jxx::NEW<::jxx::util::concurrent::atomic::AtomicIntegerArray>(3);
    value->set(0, -7); value->set(1, 0); value->set(2, 42);
    auto restored = roundTrip(value);
    ASSERT_EQ(3, restored->length());
    EXPECT_EQ(-7, restored->get(0)); EXPECT_EQ(0, restored->get(1)); EXPECT_EQ(42, restored->get(2));
}

TEST(JxxSerializableUtilityBatch, RoundTripsAtomicLongArray) {
    ::jxx::initialize();
    auto value = ::jxx::NEW<::jxx::util::concurrent::atomic::AtomicLongArray>(3);
    value->set(0, -9000000000000LL); value->set(1, 0); value->set(2, 9000000000000LL);
    auto restored = roundTrip(value);
    ASSERT_EQ(3, restored->length());
    EXPECT_EQ(-9000000000000LL, restored->get(0)); EXPECT_EQ(9000000000000LL, restored->get(2));
}
