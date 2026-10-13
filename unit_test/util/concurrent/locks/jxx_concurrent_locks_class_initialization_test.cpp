#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractOwnableSynchronizer.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractQueuedLongSynchronizer.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractQueuedSynchronizer.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.Condition.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.Lock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.LockSupport.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReadWriteLock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantLock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantReadWriteLock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.StampedLock.h"

TEST(JxxConcurrentLocksClassInitialization, MetadataTypesAreInitialized)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::util::concurrent::locks::AbstractOwnableSynchronizer::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::AbstractQueuedLongSynchronizer::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::AbstractQueuedSynchronizer::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::Condition::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::Lock::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::LockSupport::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::ReadWriteLock::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::ReentrantLock::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::ReentrantReadWriteLock::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::util::concurrent::locks::StampedLock::JxxClassInfoMarker::Class(), nullptr);
}

TEST(JxxConcurrentLocksClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto lockBefore =
        ::jxx::util::concurrent::locks::Lock::JxxClassInfoMarker::Class();
    const auto reentrantLockBefore =
        ::jxx::util::concurrent::locks::ReentrantLock::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::util::concurrent::locks::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::concurrent::locks::initializeClasses());

    EXPECT_EQ(
        lockBefore,
        ::jxx::util::concurrent::locks::Lock::JxxClassInfoMarker::Class());
    EXPECT_EQ(
        reentrantLockBefore,
        ::jxx::util::concurrent::locks::ReentrantLock::JxxClassInfoMarker::Class());
}
