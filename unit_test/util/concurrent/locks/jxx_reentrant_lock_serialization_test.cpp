#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantLock.h"

namespace {
using Lock = ::jxx::util::concurrent::locks::ReentrantLock;

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

TEST(ReentrantLockSerialization, PreservesFairnessButRestoresUnlockedState) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<Lock>(true);
    original->lock();
    original->lock();
    ASSERT_TRUE(original->isLocked());
    ASSERT_EQ(2, original->getHoldCount());

    auto restored = roundTrip(original);

    original->unlock();
    original->unlock();
    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(restored->isFair());
    EXPECT_FALSE(restored->isLocked());
    EXPECT_EQ(0, restored->getHoldCount());
    EXPECT_FALSE(restored->hasQueuedThreads());
    EXPECT_EQ(0, restored->getQueueLength());
    EXPECT_TRUE(restored->tryLock());
    EXPECT_TRUE(restored->isHeldByCurrentThread());
    restored->unlock();
}

TEST(ReentrantLockSerialization, RoundTripsTypedReferenceArrays) {
    using Array = ::jxx::lang::JxxArray<::jxx::Ptr<Lock>, 1U>;
    using Matrix = ::jxx::lang::JxxArray<::jxx::Ptr<Lock>, 2U>;
    using Row = Matrix::SubArray;
    ::jxx::initialize();

    auto array = ::jxx::NEW<Array>(1);
    (*array)[0] = ::jxx::NEW<Lock>(false);
    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    EXPECT_FALSE((*restoredArray)[0]->isFair());
    EXPECT_FALSE((*restoredArray)[0]->isLocked());

    auto matrix = ::jxx::NEW<Matrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<Lock>(true);
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_TRUE((*restoredRow)[0]->isFair());
    EXPECT_FALSE((*restoredRow)[0]->isLocked());
}

TEST(ReentrantLockSerialization, NoDataRestoresUnlockedNonfairState) {
    auto value = ::jxx::NEW<Lock>(true);
    value->readObjectNoData();
    EXPECT_FALSE(value->isFair());
    EXPECT_FALSE(value->isLocked());
    EXPECT_EQ(0, value->getHoldCount());
    EXPECT_FALSE(value->hasQueuedThreads());
}
} // namespace
