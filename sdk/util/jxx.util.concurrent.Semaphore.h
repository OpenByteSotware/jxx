#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <limits>
#include <mutex>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class Semaphore final
    : public ::jxx::lang::ClassBase<
          Semaphore,
          ::jxx::lang::Object,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Semaphore, JxxSuper, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<
        Semaphore, JxxSuper, ::jxx::io::SerializableI>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    explicit Semaphore(::jxx::lang::jint permits)
        : Semaphore(permits, false) {}

    Semaphore(::jxx::lang::jint permits, ::jxx::lang::jbool fair)
        : Super(), permits_(permits), fair_(fair) {
        if (permits < 0) throw ::jxx::lang::IllegalArgumentException();
    }

    void acquire() { acquire(1); }

    void acquire(::jxx::lang::jint permits) {
        validatePermits_(permits);
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        auto current = ::jxx::lang::Thread::currentThread();
        const auto id = current ? current->getId() : 0;
        std::unique_lock<std::mutex> lock(mutex_);
        if (fair_) waiters_.push_back(id);
        for (;;) {
            const auto eligible = permits_ >= permits &&
                (!fair_ || (!waiters_.empty() && waiters_.front() == id));
            if (eligible) break;
            condition_.wait_for(lock, std::chrono::milliseconds(10));
            if (::jxx::lang::Thread::interrupted()) {
                if (fair_) eraseWaiter_(id);
                condition_.notify_all();
                throw ::jxx::lang::InterruptedException();
            }
        }
        if (fair_) waiters_.pop_front();
        permits_ -= permits;
        condition_.notify_all();
    }

    void acquireUninterruptibly() { acquireUninterruptibly(1); }

    void acquireUninterruptibly(::jxx::lang::jint permits) {
        validatePermits_(permits);
        auto current = ::jxx::lang::Thread::currentThread();
        ::jxx::lang::jbool interrupted = false;
        std::unique_lock<std::mutex> lock(mutex_);
        while (permits_ < permits) {
            condition_.wait_for(lock, std::chrono::milliseconds(10));
            if (current && current->isInterrupted()) interrupted = true;
        }
        permits_ -= permits;
        lock.unlock();
        if (interrupted && current) current->interrupt();
    }

    ::jxx::lang::jbool tryAcquire() { return tryAcquire(1); }

    ::jxx::lang::jbool tryAcquire(::jxx::lang::jint permits) {
        validatePermits_(permits);
        std::lock_guard<std::mutex> lock(mutex_);
        if (permits_ < permits) return false;
        permits_ -= permits;
        return true;
    }

    ::jxx::lang::jbool tryAcquire(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        return tryAcquire(1, timeout, unit);
    }

    ::jxx::lang::jbool tryAcquire(
        ::jxx::lang::jint permits,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        validatePermits_(permits);
        if (!unit) throw ::jxx::lang::NullPointerException();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        const auto deadline = std::chrono::steady_clock::now() +
            unit->toChrono(timeout);
        auto current = ::jxx::lang::Thread::currentThread();
        const auto id = current ? current->getId() : 0;
        std::unique_lock<std::mutex> lock(mutex_);
        if (fair_) waiters_.push_back(id);
        for (;;) {
            const auto eligible = permits_ >= permits &&
                (!fair_ || (!waiters_.empty() && waiters_.front() == id));
            if (eligible) break;
            const auto now = std::chrono::steady_clock::now();
            if (now >= deadline) {
                if (fair_) eraseWaiter_(id);
                condition_.notify_all();
                return false;
            }
            condition_.wait_for(lock, std::min(
                deadline - now,
                std::chrono::steady_clock::duration(
                    std::chrono::milliseconds(10))));
            if (::jxx::lang::Thread::interrupted()) {
                if (fair_) eraseWaiter_(id);
                condition_.notify_all();
                throw ::jxx::lang::InterruptedException();
            }
        }
        if (fair_) waiters_.pop_front();
        permits_ -= permits;
        condition_.notify_all();
        return true;
    }

    void release() { release(1); }

    void release(::jxx::lang::jint permits) {
        validatePermits_(permits);
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (permits_ > std::numeric_limits<::jxx::lang::jint>::max() - permits) {
                throw ::jxx::lang::Error();
            }
            permits_ += permits;
        }
        condition_.notify_all();
    }

    ::jxx::lang::jint availablePermits() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return permits_;
    }

    ::jxx::lang::jint drainPermits() {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto result = permits_;
        permits_ = 0;
        return result;
    }

    ::jxx::lang::jbool isFair() const noexcept { return fair_; }

    ::jxx::lang::jbool hasQueuedThreads() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return !waiters_.empty();
    }

    ::jxx::lang::jint getQueueLength() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return static_cast<::jxx::lang::jint>(waiters_.size());
    }

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {(void)out;}
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {(void)in;}
    void readObjectNoData() override {}

private:
    void eraseWaiter_(::jxx::lang::jlong id) {
        auto it = std::find(waiters_.begin(), waiters_.end(), id);
        if (it != waiters_.end()) waiters_.erase(it);
    }

    static void validatePermits_(::jxx::lang::jint permits) {
        if (permits < 0) throw ::jxx::lang::IllegalArgumentException();
    }

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::lang::jint permits_;
    ::jxx::lang::jbool fair_;
    std::deque<::jxx::lang::jlong> waiters_;
};

} // namespace jxx::util::concurrent
