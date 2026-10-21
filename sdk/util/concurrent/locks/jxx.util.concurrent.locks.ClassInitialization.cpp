#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
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

namespace jxx::util::concurrent::locks {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)AbstractOwnableSynchronizer::JxxClassInfoMarker::Class();
        (void)AbstractQueuedLongSynchronizer::JxxClassInfoMarker::Class();
        (void)AbstractQueuedSynchronizer::JxxClassInfoMarker::Class();
        (void)Condition::JxxClassInfoMarker::Class();
        (void)Lock::JxxClassInfoMarker::Class();
        (void)LockSupport::JxxClassInfoMarker::Class();
        (void)ReadWriteLock::JxxClassInfoMarker::Class();
        (void)ReentrantLock::JxxClassInfoMarker::Class();
        ::jxx::io::serialization::registerSerializableClass<ReentrantLock>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ReentrantLock, 1U, 2U>();
        (void)ReentrantReadWriteLock::JxxClassInfoMarker::Class();
        ::jxx::io::serialization::registerSerializableClass<
            ReentrantReadWriteLock>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ReentrantReadWriteLock, 1U, 2U>();
        (void)StampedLock::JxxClassInfoMarker::Class();
        ::jxx::io::serialization::registerSerializableClass<StampedLock>(1);
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            StampedLock, 1U, 2U>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::util::concurrent::locks
