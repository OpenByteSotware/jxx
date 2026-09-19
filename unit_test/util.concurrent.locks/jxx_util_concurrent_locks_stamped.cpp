#include <gtest/gtest.h>
#include <future>

#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.StampedLock.h"

namespace {
using jxx::util::concurrent::locks::StampedLock;

TEST(StampedLockTest, WriteInvalidatesOptimisticRead) {
    auto lock = jxx::NEW<StampedLock>();
    const auto optimistic = lock->tryOptimisticRead();
    EXPECT_TRUE(lock->validate(optimistic));
    const auto write = lock->writeLock();
    EXPECT_FALSE(lock->validate(optimistic));
    lock->unlockWrite(write);
}

TEST(StampedLockTest, MultipleReadersArePermitted) {
    auto lock = jxx::NEW<StampedLock>();
    const auto first = lock->readLock();
    auto second = std::async(std::launch::async, [&] {
        const auto stamp = lock->readLock();
        lock->unlockRead(stamp);
        return true;
    });
    EXPECT_TRUE(second.get());
    lock->unlockRead(first);
}

TEST(StampedLockTest, WrongUnlockModeThrows) {
    auto lock = jxx::NEW<StampedLock>();
    const auto read = lock->readLock();
    EXPECT_THROW(lock->unlockWrite(read), jxx::lang::IllegalMonitorStateException);
    lock->unlockRead(read);
}

TEST(StampedLockTest, ViewLocksRoundTrip) {
    auto lock = jxx::NEW<StampedLock>();
    auto read = lock->asReadLock();
    read->lock();
    read->unlock();
    auto write = lock->asWriteLock();
    write->lock();
    write->unlock();
}
}
