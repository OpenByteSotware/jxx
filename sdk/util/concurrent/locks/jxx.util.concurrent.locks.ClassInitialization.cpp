#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
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
        (void)ReentrantReadWriteLock::JxxClassInfoMarker::Class();
        (void)StampedLock::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::util::concurrent::locks
