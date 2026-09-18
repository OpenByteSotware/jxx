#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>

#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractOwnableSynchronizer.h"

namespace jxx::util::concurrent::locks {

class AbstractQueuedLongSynchronizer
    : public ::jxx::lang::ClassBase<
          AbstractQueuedLongSynchronizer,
          AbstractOwnableSynchronizer> {
public:
    using JxxSuper = AbstractOwnableSynchronizer;
    using Super = ::jxx::lang::ClassBase<AbstractQueuedLongSynchronizer, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<AbstractQueuedLongSynchronizer, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    ~AbstractQueuedLongSynchronizer() override = default;

    void acquire(::jxx::lang::jlong arg);
    void acquireInterruptibly(::jxx::lang::jlong arg);
    ::jxx::lang::jbool tryAcquireNanos(::jxx::lang::jlong arg, ::jxx::lang::jlong nanosTimeout);
    ::jxx::lang::jbool release(::jxx::lang::jlong arg);
    void acquireShared(::jxx::lang::jlong arg);
    void acquireSharedInterruptibly(::jxx::lang::jlong arg);
    ::jxx::lang::jbool tryAcquireSharedNanos(::jxx::lang::jlong arg, ::jxx::lang::jlong nanosTimeout);
    ::jxx::lang::jbool releaseShared(::jxx::lang::jlong arg);

    ::jxx::lang::jbool hasQueuedThreads() const;
    ::jxx::lang::jint getQueueLength() const;

protected:
    AbstractQueuedLongSynchronizer();
    ::jxx::lang::jlong getState() const noexcept;
    void setState(::jxx::lang::jlong value) noexcept;
    ::jxx::lang::jbool compareAndSetState(::jxx::lang::jlong expected, ::jxx::lang::jlong update) noexcept;
    virtual ::jxx::lang::jbool tryAcquire(::jxx::lang::jlong arg);
    virtual ::jxx::lang::jbool tryRelease(::jxx::lang::jlong arg);
    virtual ::jxx::lang::jlong tryAcquireShared(::jxx::lang::jlong arg);
    virtual ::jxx::lang::jbool tryReleaseShared(::jxx::lang::jlong arg);
    virtual ::jxx::lang::jbool isHeldExclusively() const;

private:
    struct Waiter final {
        std::thread::id nativeId;
        ::jxx::Ptr<::jxx::lang::Thread> thread;
        ::jxx::lang::jbool shared = false;
        ::jxx::lang::jbool cancelled = false;
    };

    ::jxx::lang::jbool queuedAcquire_(::jxx::lang::jlong arg,
                                      ::jxx::lang::jbool shared,
                                      ::jxx::lang::jbool interruptible,
                                      const std::chrono::steady_clock::time_point* deadline);
    void removeCancelled_();

    mutable std::mutex queueMutex_;
    std::condition_variable queueChanged_;
    std::deque<std::shared_ptr<Waiter>> queue_;
    std::atomic<::jxx::lang::jlong> state_{0};
};

} // namespace jxx::util::concurrent::locks
