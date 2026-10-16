#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/concurrent/jxx.util.concurrent.Semaphore.h"

namespace {

using Semaphore = ::jxx::util::concurrent::Semaphore;

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

TEST(SemaphoreSerialization, RoundTripsPermitsAndFairness) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Semaphore>(5, true);
    original->acquire(2);

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(3, restored->availablePermits());
    EXPECT_TRUE(restored->isFair());
    EXPECT_FALSE(restored->hasQueuedThreads());
    EXPECT_EQ(0, restored->getQueueLength());
    EXPECT_TRUE(restored->tryAcquire(3));
    EXPECT_EQ(0, restored->availablePermits());
    restored->release(2);
    EXPECT_EQ(2, restored->availablePermits());
}

TEST(SemaphoreSerialization, RoundTripsTypedReferenceArrays) {
    using Array = ::jxx::lang::JxxArray<::jxx::Ptr<Semaphore>, 1U>;
    using Matrix = ::jxx::lang::JxxArray<::jxx::Ptr<Semaphore>, 2U>;
    using Row = Matrix::SubArray;
    ::jxx::initialize();

    auto array = ::jxx::NEW<Array>(1);
    (*array)[0] = ::jxx::NEW<Semaphore>(4, false);
    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    EXPECT_EQ(4, (*restoredArray)[0]->availablePermits());

    auto matrix = ::jxx::NEW<Matrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<Semaphore>(6, true);
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ(6, (*restoredRow)[0]->availablePermits());
    EXPECT_TRUE((*restoredRow)[0]->isFair());
}

TEST(SemaphoreSerialization, NoDataRestoresEmptyNonfairState) {
    auto value = ::jxx::NEW<Semaphore>(7, true);
    value->readObjectNoData();
    EXPECT_EQ(0, value->availablePermits());
    EXPECT_FALSE(value->isFair());
    EXPECT_FALSE(value->hasQueuedThreads());
    EXPECT_EQ(0, value->getQueueLength());
}

} // namespace
