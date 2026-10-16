#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.DoubleAdder.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.LongAdder.h"

namespace {

using DoubleAdder = ::jxx::util::concurrent::atomic::DoubleAdder;
using LongAdder = ::jxx::util::concurrent::atomic::LongAdder;

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

TEST(AdderSerialization, RoundTripsLongAdderSum) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<LongAdder>();
    original->add(40);
    original->add(-3);
    original->increment();

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(38, restored->sum());
    restored->add(4);
    EXPECT_EQ(42, restored->sum());
}

TEST(AdderSerialization, RoundTripsDoubleAdderSumAndSpecialValues) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<DoubleAdder>();
    original->add(10.5);
    original->add(-0.25);

    auto signedZero = ::jxx::NEW<DoubleAdder>();
    signedZero->add(-0.0);

    auto restoredZero =
        roundTrip(signedZero);

    ASSERT_NE(restoredZero, nullptr);

    EXPECT_DOUBLE_EQ(
        0.0,
        restoredZero->sum());

    EXPECT_FALSE(
        std::signbit(
            restoredZero->sum()));
}

TEST(AdderSerialization, RoundTripsTypedReferenceArrays) {
    using LongArray =
        ::jxx::lang::JxxArray<::jxx::Ptr<LongAdder>, 1U>;
    using DoubleMatrix =
        ::jxx::lang::JxxArray<::jxx::Ptr<DoubleAdder>, 2U>;
    using DoubleRow = DoubleMatrix::SubArray;

    ::jxx::initialize();
    auto longs = ::jxx::NEW<LongArray>(1);
    (*longs)[0] = ::jxx::NEW<LongAdder>();
    (*longs)[0]->add(7);
    auto restoredLongs = roundTrip(longs);
    ASSERT_NE(restoredLongs, nullptr);
    EXPECT_EQ(7, (*restoredLongs)[0]->sum());

    auto matrix = ::jxx::NEW<DoubleMatrix>(1);
    auto row = ::jxx::NEW<DoubleRow>(1);
    (*row)[0] = ::jxx::NEW<DoubleAdder>();
    (*row)[0]->add(8.5);
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_DOUBLE_EQ(8.5, (*restoredRow)[0]->sum());
}

TEST(AdderSerialization, NoDataRestoresZero) {
    auto longAdder = ::jxx::NEW<LongAdder>();
    longAdder->add(99);
    longAdder->readObjectNoData();
    EXPECT_EQ(0, longAdder->sum());

    auto doubleAdder = ::jxx::NEW<DoubleAdder>();
    doubleAdder->add(99.5);
    doubleAdder->readObjectNoData();
    EXPECT_DOUBLE_EQ(0.0, doubleAdder->sum());
}

} // namespace
