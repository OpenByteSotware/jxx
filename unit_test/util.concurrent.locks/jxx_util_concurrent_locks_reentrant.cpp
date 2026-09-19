#include <gtest/gtest.h>
#include <atomic>
#include <future>

#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantLock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantReadWriteLock.h"

namespace {
using jxx::util::concurrent::locks::ReentrantLock;
using jxx::util::concurrent::locks::ReentrantReadWriteLock;

TEST(ReentrantLockTest, TracksReentrantHoldCount) {
    auto lock = jxx::NEW<ReentrantLock>();
    lock->lock();
    lock->lock();
    EXPECT_TRUE(lock->isHeldByCurrentThread());
    EXPECT_TRUE(lock->isLocked());
    EXPECT_EQ(lock->getHoldCount(), 2);
    lock->unlock();
    EXPECT_EQ(lock->getHoldCount(), 1);
    lock->unlock();
    EXPECT_FALSE(lock->isLocked());
}

TEST(ReentrantLockTest, UnlockByNonOwnerThrows) {
    auto lock = jxx::NEW<ReentrantLock>();
    lock->lock();
    auto result = std::async(std::launch::async, [&] {
        try {
            lock->unlock();
            return false;
        } catch (const jxx::lang::IllegalMonitorStateException&) {
            return true;
        }
    });
    EXPECT_TRUE(result.get());
    lock->unlock();
}

TEST(ReentrantReadWriteLockTest, ReadLocksCanBeShared) {
    auto rw = jxx::NEW<ReentrantReadWriteLock>();
    auto read = rw->readLock();
    read->lock();
    EXPECT_EQ(rw->getReadLockCount(), 1);
    auto second = std::async(std::launch::async, [&] {
        auto other = rw->readLock();
        other->lock();
        const auto count = rw->getReadLockCount();
        other->unlock();
        return count;
    });
    EXPECT_GE(second.get(), 2);
    read->unlock();
    EXPECT_EQ(rw->getReadLockCount(), 0);
}

TEST(ReentrantReadWriteLockTest, WriteLockIsExclusive) {
    auto rw = jxx::NEW<ReentrantReadWriteLock>();
    auto write = rw->writeLock();
    write->lock();
    EXPECT_TRUE(rw->isWriteLocked());
    EXPECT_TRUE(rw->isWriteLockedByCurrentThread());
    EXPECT_EQ(rw->getWriteHoldCount(), 1);
    write->unlock();
    EXPECT_FALSE(rw->isWriteLocked());
}
}
