#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantReadWriteLock.h"

namespace {
using ReadWriteLock =
    ::jxx::util::concurrent::locks::ReentrantReadWriteLock;

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

TEST(ReentrantReadWriteLockSerialization,
    PreservesFairnessButRestoresUnlockedState) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<ReadWriteLock>(true);
    auto originalWrite = original->writeLock();
    originalWrite->lock();
    ASSERT_TRUE(original->isWriteLocked());
    ASSERT_EQ(1, original->getWriteHoldCount());

    auto restored = roundTrip(original);

    originalWrite->unlock();
    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(restored->isFair());
    EXPECT_FALSE(restored->isWriteLocked());
    EXPECT_EQ(0, restored->getWriteHoldCount());
    EXPECT_EQ(0, restored->getReadHoldCount());
    EXPECT_EQ(0, restored->getReadLockCount());
    EXPECT_FALSE(restored->hasQueuedThreads());
    EXPECT_EQ(0, restored->getQueueLength());

    auto restoredRead = restored->readLock();
    ASSERT_TRUE(restoredRead->tryLock());
    EXPECT_EQ(1, restored->getReadHoldCount());
    restoredRead->unlock();
}

TEST(ReentrantReadWriteLockSerialization, RoundTripsTypedReferenceArrays) {
    using Array =
        ::jxx::lang::JxxArray<::jxx::Ptr<ReadWriteLock>, 1U>;
    using Matrix =
        ::jxx::lang::JxxArray<::jxx::Ptr<ReadWriteLock>, 2U>;
    using Row = Matrix::SubArray;
    ::jxx::initialize();

    auto array = ::jxx::NEW<Array>(1);
    (*array)[0] = ::jxx::NEW<ReadWriteLock>(false);
    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    EXPECT_FALSE((*restoredArray)[0]->isFair());
    EXPECT_FALSE((*restoredArray)[0]->isWriteLocked());

    auto matrix = ::jxx::NEW<Matrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<ReadWriteLock>(true);
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_TRUE((*restoredRow)[0]->isFair());
    EXPECT_EQ(0, (*restoredRow)[0]->getReadLockCount());
}

TEST(ReentrantReadWriteLockSerialization,
    NoDataRestoresUnlockedNonfairState) {
    auto value = ::jxx::NEW<ReadWriteLock>(true);
    value->readObjectNoData();
    EXPECT_FALSE(value->isFair());
    EXPECT_FALSE(value->isWriteLocked());
    EXPECT_EQ(0, value->getWriteHoldCount());
    EXPECT_EQ(0, value->getReadHoldCount());
    EXPECT_EQ(0, value->getReadLockCount());
    EXPECT_FALSE(value->hasQueuedThreads());
}
} // namespace
