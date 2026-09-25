#include <algorithm>
#include <chrono>
#include <limits>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.Semaphore.h"

namespace jxx::util::concurrent {

Semaphore::Semaphore(::jxx::lang::jint permits)
    : Semaphore(permits, false) {
}

Semaphore::Semaphore(
    ::jxx::lang::jint permits,
    ::jxx::lang::jbool fair)
    : Super(), permits_(permits), fair_(fair) {
    if (permits < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

void Semaphore::validatePermits_(::jxx::lang::jint permits) {
    if (permits < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

void Semaphore::eraseWaiter_(std::uint64_t ticket) {
    const auto iterator = std::find_if(
        waiters_.begin(),
        waiters_.end(),
        [ticket](const Waiter& waiter) {
            return waiter.ticket == ticket;
        });
    if (iterator != waiters_.end()) {
        waiters_.erase(iterator);
    }
}

::jxx::lang::jbool Semaphore::eligible_(
    std::uint64_t ticket,
    ::jxx::lang::jint permits) const {
    if (permits_ < permits) {
        return false;
    }
    return !fair_ ||
        (!waiters_.empty() && waiters_.front().ticket == ticket);
}

void Semaphore::acquire() {
    acquire(1);
}

void Semaphore::acquire(::jxx::lang::jint permits) {
    validatePermits_(permits);
    if (::jxx::lang::Thread::interrupted()) {
        throw ::jxx::lang::InterruptedException();
    }
    std::unique_lock<std::mutex> lock(mutex_);
    const auto ticket = fair_ ? nextTicket_++ : 0U;
    if (fair_) {
        waiters_.push_back({ticket, permits});
    }
    for (;;) {
        if (eligible_(ticket, permits)) {
            break;
        }
        condition_.wait_for(lock, std::chrono::milliseconds(10));
        if (::jxx::lang::Thread::interrupted()) {
            if (fair_) {
                eraseWaiter_(ticket);
            }
            condition_.notify_all();
            throw ::jxx::lang::InterruptedException();
        }
    }
    if (fair_) {
        waiters_.pop_front();
    }
    permits_ -= permits;
    condition_.notify_all();
}

void Semaphore::acquireUninterruptibly() {
    acquireUninterruptibly(1);
}

void Semaphore::acquireUninterruptibly(::jxx::lang::jint permits) {
    validatePermits_(permits);
    auto current = ::jxx::lang::Thread::currentThread();
    ::jxx::lang::jbool interrupted =
        ::jxx::lang::Thread::interrupted();
    std::unique_lock<std::mutex> lock(mutex_);
    const auto ticket = fair_ ? nextTicket_++ : 0U;
    if (fair_) {
        waiters_.push_back({ticket, permits});
    }
    while (!eligible_(ticket, permits)) {
        condition_.wait_for(lock, std::chrono::milliseconds(10));
        if (::jxx::lang::Thread::interrupted()) {
            interrupted = true;
        }
    }
    if (fair_) {
        waiters_.pop_front();
    }
    permits_ -= permits;
    lock.unlock();
    condition_.notify_all();
    if (interrupted && current != nullptr) {
        current->interrupt();
    }
}

::jxx::lang::jbool Semaphore::tryAcquire() {
    return tryAcquire(1);
}

::jxx::lang::jbool Semaphore::tryAcquire(::jxx::lang::jint permits) {
    validatePermits_(permits);
    std::lock_guard<std::mutex> lock(mutex_);
    if (permits_ < permits) {
        return false;
    }
    permits_ -= permits;
    return true;
}

::jxx::lang::jbool Semaphore::tryAcquire(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    return tryAcquire(1, timeout, unit);
}

::jxx::lang::jbool Semaphore::tryAcquire(
    ::jxx::lang::jint permits,
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    validatePermits_(permits);
    if (unit == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (::jxx::lang::Thread::interrupted()) {
        throw ::jxx::lang::InterruptedException();
    }
    std::unique_lock<std::mutex> lock(mutex_);
    if (timeout <= 0) {
        if (permits_ < permits || (fair_ && !waiters_.empty())) {
            return false;
        }
        permits_ -= permits;
        return true;
    }
    const auto duration = unit->toChrono(timeout);
    const auto now = std::chrono::steady_clock::now();
    const auto maximum = std::chrono::steady_clock::time_point::max() - now;
    const auto deadline = duration >= maximum
        ? std::chrono::steady_clock::time_point::max()
        : now + duration;
    const auto ticket = fair_ ? nextTicket_++ : 0U;
    if (fair_) {
        waiters_.push_back({ticket, permits});
    }
    for (;;) {
        if (eligible_(ticket, permits)) {
            break;
        }
        const auto current = std::chrono::steady_clock::now();
        if (current >= deadline) {
            if (fair_) {
                eraseWaiter_(ticket);
            }
            condition_.notify_all();
            return false;
        }
        condition_.wait_for(
            lock,
            std::min(
                deadline - current,
                std::chrono::steady_clock::duration(
                    std::chrono::milliseconds(10))));
        if (::jxx::lang::Thread::interrupted()) {
            if (fair_) {
                eraseWaiter_(ticket);
            }
            condition_.notify_all();
            throw ::jxx::lang::InterruptedException();
        }
    }
    if (fair_) {
        waiters_.pop_front();
    }
    permits_ -= permits;
    condition_.notify_all();
    return true;
}

void Semaphore::release() {
    release(1);
}

void Semaphore::release(::jxx::lang::jint permits) {
    validatePermits_(permits);
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (permits_ >
            std::numeric_limits<::jxx::lang::jint>::max() - permits) {
            throw ::jxx::lang::Error();
        }
        permits_ += permits;
    }
    condition_.notify_all();
}

::jxx::lang::jint Semaphore::availablePermits() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return permits_;
}

::jxx::lang::jint Semaphore::drainPermits() {
    std::lock_guard<std::mutex> lock(mutex_);
    const auto result = permits_;
    permits_ = 0;
    return result;
}

::jxx::lang::jbool Semaphore::isFair() const noexcept {
    return fair_;
}

::jxx::lang::jbool Semaphore::hasQueuedThreads() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !waiters_.empty();
}

::jxx::lang::jint Semaphore::getQueueLength() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<::jxx::lang::jint>(waiters_.size());
}

void Semaphore::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    (void)out;
}

void Semaphore::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    (void)in;
}

void Semaphore::readObjectNoData() {
}

} // namespace jxx::util::concurrent
