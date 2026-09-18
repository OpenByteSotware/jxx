#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractQueuedSynchronizer.h"

#include <algorithm>
#include <chrono>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"

namespace jxx::util::concurrent::locks {

::jxx::Ptr<::jxx::lang::ClassAny> AbstractQueuedSynchronizer::Class() {
    return JxxClassInfoMarker::Class();
}

AbstractQueuedSynchronizer::AbstractQueuedSynchronizer() : Super() {}

::jxx::lang::jint AbstractQueuedSynchronizer::getState() const noexcept {
    return state_.load(std::memory_order_acquire);
}

void AbstractQueuedSynchronizer::setState(::jxx::lang::jint value) noexcept {
    state_.store(value, std::memory_order_release);
}

::jxx::lang::jbool AbstractQueuedSynchronizer::compareAndSetState(::jxx::lang::jint expected,
                                               ::jxx::lang::jint update) noexcept {
    return state_.compare_exchange_strong(
        expected, update, std::memory_order_acq_rel);
}

void AbstractQueuedSynchronizer::acquire(::jxx::lang::jint arg) {
    if (tryAcquire(arg)) return;
    const auto interrupted = queuedAcquire_(arg, false, false, nullptr);
    if (interrupted) {
        auto current = ::jxx::lang::Thread::currentThread();
        if (current) current->interrupt();
    }
}

void AbstractQueuedSynchronizer::acquireInterruptibly(::jxx::lang::jint arg) {
    if (::jxx::lang::Thread::interrupted())
        throw ::jxx::lang::InterruptedException();
    if (!tryAcquire(arg))
        (void)queuedAcquire_(arg, false, true, nullptr);
}

::jxx::lang::jbool AbstractQueuedSynchronizer::tryAcquireNanos(::jxx::lang::jint arg,
                                            ::jxx::lang::jlong nanos) {
    if (::jxx::lang::Thread::interrupted())
        throw ::jxx::lang::InterruptedException();
    if (tryAcquire(arg)) return true;
    if (nanos <= 0) return false;
    const auto deadline = std::chrono::steady_clock::now() +
        std::chrono::nanoseconds(nanos);
    return queuedAcquire_(arg, false, true, &deadline);
}

::jxx::lang::jbool AbstractQueuedSynchronizer::release(::jxx::lang::jint arg) {
    if (!tryRelease(arg)) return false;
    queueChanged_.notify_all();
    return true;
}

void AbstractQueuedSynchronizer::acquireShared(::jxx::lang::jint arg) {
    if (tryAcquireShared(arg) >= 0) return;
    const auto interrupted = queuedAcquire_(arg, true, false, nullptr);
    if (interrupted) {
        auto current = ::jxx::lang::Thread::currentThread();
        if (current) current->interrupt();
    }
}

void AbstractQueuedSynchronizer::acquireSharedInterruptibly(::jxx::lang::jint arg) {
    if (::jxx::lang::Thread::interrupted())
        throw ::jxx::lang::InterruptedException();
    if (tryAcquireShared(arg) < 0)
        (void)queuedAcquire_(arg, true, true, nullptr);
}

::jxx::lang::jbool AbstractQueuedSynchronizer::tryAcquireSharedNanos(
    ::jxx::lang::jint arg, ::jxx::lang::jlong nanos) {
    if (::jxx::lang::Thread::interrupted())
        throw ::jxx::lang::InterruptedException();
    if (tryAcquireShared(arg) >= 0) return true;
    if (nanos <= 0) return false;
    const auto deadline = std::chrono::steady_clock::now() +
        std::chrono::nanoseconds(nanos);
    return queuedAcquire_(arg, true, true, &deadline);
}

::jxx::lang::jbool AbstractQueuedSynchronizer::releaseShared(::jxx::lang::jint arg) {
    if (!tryReleaseShared(arg)) return false;
    queueChanged_.notify_all();
    return true;
}

::jxx::lang::jbool AbstractQueuedSynchronizer::queuedAcquire_(
    ::jxx::lang::jint arg,
    ::jxx::lang::jbool shared,
    ::jxx::lang::jbool interruptible,
    const std::chrono::steady_clock::time_point* deadline) {
    auto waiter = std::make_shared<Waiter>();
    waiter->nativeId = std::this_thread::get_id();
    waiter->thread = ::jxx::lang::Thread::currentThread();
    waiter->shared = shared;

    std::unique_lock<std::mutex> lock(queueMutex_);
    queue_.push_back(waiter);
    ::jxx::lang::jbool interrupted = false;

    for (;;) {
        removeCancelled_();
        const auto atHead = !queue_.empty() && queue_.front() == waiter;
        if (atHead && (shared ? tryAcquireShared(arg) >= 0 : tryAcquire(arg))) {
            queue_.pop_front();
            queueChanged_.notify_all();
            return interrupted;
        }

        const auto nowInterrupted = waiter->thread && waiter->thread->isInterrupted();
        if (nowInterrupted) {
            if (interruptible) {
                (void)::jxx::lang::Thread::interrupted();
                waiter->cancelled = true;
                removeCancelled_();
                queueChanged_.notify_all();
                throw ::jxx::lang::InterruptedException();
            }
            interrupted = true;
        }

        if (deadline) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= *deadline) {
                waiter->cancelled = true;
                removeCancelled_();
                queueChanged_.notify_all();
                return false;
            }
            queueChanged_.wait_for(
                lock,
                std::min(*deadline - now,
                         std::chrono::steady_clock::duration(
                             std::chrono::milliseconds(10))));
        } else {
            queueChanged_.wait_for(lock, std::chrono::milliseconds(10));
        }
    }
}

void AbstractQueuedSynchronizer::removeCancelled_() {
    queue_.erase(std::remove_if(queue_.begin(), queue_.end(),
        [](const std::shared_ptr<Waiter>& waiter) {
            return waiter->cancelled;
        }), queue_.end());
}

::jxx::lang::jbool AbstractQueuedSynchronizer::hasQueuedThreads() const {
    std::lock_guard<std::mutex> lock(queueMutex_);
    return std::any_of(queue_.begin(), queue_.end(),
        [](const std::shared_ptr<Waiter>& waiter) {
            return !waiter->cancelled;
        });
}

::jxx::lang::jint AbstractQueuedSynchronizer::getQueueLength() const {
    std::lock_guard<std::mutex> lock(queueMutex_);
    return static_cast<::jxx::lang::jint>(std::count_if(
        queue_.begin(), queue_.end(),
        [](const std::shared_ptr<Waiter>& waiter) {
            return !waiter->cancelled;
        }));
}

::jxx::lang::jbool AbstractQueuedSynchronizer::tryAcquire(::jxx::lang::jint) {
    throw ::jxx::lang::UnsupportedOperationException();
}
::jxx::lang::jbool AbstractQueuedSynchronizer::tryRelease(::jxx::lang::jint) {
    throw ::jxx::lang::UnsupportedOperationException();
}
::jxx::lang::jint AbstractQueuedSynchronizer::tryAcquireShared(::jxx::lang::jint) {
    throw ::jxx::lang::UnsupportedOperationException();
}
::jxx::lang::jbool AbstractQueuedSynchronizer::tryReleaseShared(::jxx::lang::jint) {
    throw ::jxx::lang::UnsupportedOperationException();
}
::jxx::lang::jbool AbstractQueuedSynchronizer::isHeldExclusively() const {
    throw ::jxx::lang::UnsupportedOperationException();
}

} // namespace jxx::util::concurrent::locks
