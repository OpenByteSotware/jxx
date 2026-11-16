#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.AtomicReference.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractQueuedLongSynchronizer.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractQueuedSynchronizer.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantLock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ReentrantReadWriteLock.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.StampedLock.h"

namespace {
template <typename T>
void expectUid(::jxx::lang::jlong expected) {
    const auto metadata =
        ::jxx::io::serialization::SerializationClassRegistry::resolve(
            T::Class()->getName()->utf8());
    EXPECT_EQ(expected, metadata.serialVersionUID);
}
}

TEST(UtilAtomicLockSerializationParity, MatchesSerializableHierarchy) {
    using Object = ::jxx::lang::Object;
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI,
        ::jxx::util::concurrent::atomic::AtomicReference<Object>>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI,
        ::jxx::util::concurrent::locks::AbstractQueuedSynchronizer>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI,
        ::jxx::util::concurrent::locks::AbstractQueuedLongSynchronizer>));
    EXPECT_FALSE((std::is_base_of_v<::jxx::io::SerializableI,
        ::jxx::util::concurrent::locks::StampedLock>));
}

TEST(UtilAtomicLockSerializationParity, RegistersConcreteUids) {
    ::jxx::util::concurrent::atomic::initializeClasses();
    ::jxx::util::concurrent::locks::initializeClasses();
    using Object = ::jxx::lang::Object;
    expectUid<::jxx::util::concurrent::atomic::AtomicReference<Object>>(
        static_cast<::jxx::lang::jlong>(-1848883965231344442LL));
    expectUid<::jxx::util::concurrent::locks::ReentrantLock>(
        static_cast<::jxx::lang::jlong>(7373984872572414699LL));
    expectUid<::jxx::util::concurrent::locks::ReentrantReadWriteLock>(
        static_cast<::jxx::lang::jlong>(-6992448646407690164LL));
}
