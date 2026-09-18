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

class AbstractQueuedSynchronizer
    : public ::jxx::lang::ClassBase<
          AbstractQueuedSynchronizer,
          AbstractOwnableSynchronizer> {
public:
    using JxxSuper = AbstractOwnableSynchronizer;
    using Super = ::jxx::lang::ClassBase<AbstractQueuedSynchronizer, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<AbstractQueuedSynchronizer, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    ~AbstractQueuedSynchronizer() override = default;

    void acquire(::jxx::lang::jint arg);
    void acquireInterruptibly(::jxx::lang::jint arg);
    ::jxx::lang::jbool tryAcquireNanos(::jxx::lang::jint arg, ::jxx::lang::jlong nanosTimeout);
    ::jxx::lang::jbool release(::jxx::lang::jint arg);
    void acquireShared(::jxx::lang::jint arg);
    void acquireSharedInterruptibly(::jxx::lang::jint arg);
    ::jxx::lang::jbool tryAcquireSharedNanos(::jxx::lang::jint arg, ::jxx::lang::jlong nanosTimeout);
    ::jxx::lang::jbool releaseShared(::jxx::lang::jint arg);

    ::jxx::lang::jbool hasQueuedThreads() const;
    ::jxx::lang::jint getQueueLength() const;

protected:
    AbstractQueuedSynchronizer();
    ::jxx::lang::jint getState() const noexcept;
    void setState(::jxx::lang::jint value) noexcept;
    ::jxx::lang::jbool compareAndSetState(::jxx::lang::jint expected, ::jxx::lang::jint update) noexcept;
    virtual ::jxx::lang::jbool tryAcquire(::jxx::lang::jint arg);
    virtual ::jxx::lang::jbool tryRelease(::jxx::lang::jint arg);
    virtual ::jxx::lang::jint tryAcquireShared(::jxx::lang::jint arg);
    virtual ::jxx::lang::jbool tryReleaseShared(::jxx::lang::jint arg);
    virtual ::jxx::lang::jbool isHeldExclusively() const;

private:
    struct Waiter final {
        std::thread::id nativeId;
        ::jxx::Ptr<::jxx::lang::Thread> thread;
        ::jxx::lang::jbool shared = false;
        ::jxx::lang::jbool cancelled = false;
    };

    ::jxx::lang::jbool queuedAcquire_(::jxx::lang::jint arg,
                                      ::jxx::lang::jbool shared,
                                      ::jxx::lang::jbool interruptible,
                                      const std::chrono::steady_clock::time_point* deadline);
    void removeCancelled_();

    mutable std::mutex queueMutex_;
    std::condition_variable queueChanged_;
    std::deque<std::shared_ptr<Waiter>> queue_;
    std::atomic<::jxx::lang::jint> state_{0};
};

} // namespace jxx::util::concurrent::locks
