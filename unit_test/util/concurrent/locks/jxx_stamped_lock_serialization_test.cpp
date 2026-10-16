#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.StampedLock.h"

namespace {
using StampedLock = ::jxx::util::concurrent::locks::StampedLock;

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

TEST(StampedLockSerialization, RestoresUnlockedOperationalState) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<StampedLock>();
    const auto originalWriteStamp = original->writeLock();
    ASSERT_TRUE(original->isWriteLocked());

    auto restored = roundTrip(original);

    original->unlockWrite(originalWriteStamp);
    ASSERT_NE(restored, nullptr);
    EXPECT_FALSE(restored->isWriteLocked());
    EXPECT_FALSE(restored->isReadLocked());
    EXPECT_EQ(0, restored->getReadLockCount());

    const auto optimistic = restored->tryOptimisticRead();
    EXPECT_NE(0, optimistic);
    EXPECT_TRUE(restored->validate(optimistic));

    const auto readStamp = restored->tryReadLock();
    ASSERT_NE(0, readStamp);
    EXPECT_TRUE(restored->isReadLocked());
    restored->unlockRead(readStamp);

    const auto writeStamp = restored->tryWriteLock();
    ASSERT_NE(0, writeStamp);
    EXPECT_TRUE(restored->isWriteLocked());
    restored->unlockWrite(writeStamp);
}

TEST(StampedLockSerialization, RoundTripsTypedReferenceArrays) {
    using Array = ::jxx::lang::JxxArray<::jxx::Ptr<StampedLock>, 1U>;
    using Matrix = ::jxx::lang::JxxArray<::jxx::Ptr<StampedLock>, 2U>;
    using Row = Matrix::SubArray;
    ::jxx::initialize();

    auto array = ::jxx::NEW<Array>(1);
    (*array)[0] = ::jxx::NEW<StampedLock>();
    auto restoredArray = roundTrip(array);
    ASSERT_NE(restoredArray, nullptr);
    EXPECT_FALSE((*restoredArray)[0]->isWriteLocked());

    auto matrix = ::jxx::NEW<Matrix>(1);
    auto row = ::jxx::NEW<Row>(1);
    (*row)[0] = ::jxx::NEW<StampedLock>();
    matrix->row(0) = row;
    auto restoredMatrix = roundTrip(matrix);
    ASSERT_NE(restoredMatrix, nullptr);
    const auto& restoredRow = restoredMatrix->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ(0, (*restoredRow)[0]->getReadLockCount());
}

TEST(StampedLockSerialization, NoDataRestoresFreshUnlockedState) {
    auto value = ::jxx::NEW<StampedLock>();
    value->readObjectNoData();
    EXPECT_FALSE(value->isWriteLocked());
    EXPECT_FALSE(value->isReadLocked());
    EXPECT_EQ(0, value->getReadLockCount());
    const auto stamp = value->tryOptimisticRead();
    EXPECT_NE(0, stamp);
    EXPECT_TRUE(value->validate(stamp));
}
} // namespace
