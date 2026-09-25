#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
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

    explicit Semaphore(::jxx::lang::jint permits);
    Semaphore(::jxx::lang::jint permits, ::jxx::lang::jbool fair);

    void acquire();
    void acquire(::jxx::lang::jint permits);
    void acquireUninterruptibly();
    void acquireUninterruptibly(::jxx::lang::jint permits);
    ::jxx::lang::jbool tryAcquire();
    ::jxx::lang::jbool tryAcquire(::jxx::lang::jint permits);
    ::jxx::lang::jbool tryAcquire(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit);
    ::jxx::lang::jbool tryAcquire(
        ::jxx::lang::jint permits,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit);
    void release();
    void release(::jxx::lang::jint permits);
    ::jxx::lang::jint availablePermits() const;
    ::jxx::lang::jint drainPermits();
    ::jxx::lang::jbool isFair() const noexcept;
    ::jxx::lang::jbool hasQueuedThreads() const;
    ::jxx::lang::jint getQueueLength() const;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;

private:
    struct Waiter {
        std::uint64_t ticket;
        ::jxx::lang::jint permits;
    };

    static void validatePermits_(::jxx::lang::jint permits);
    void eraseWaiter_(std::uint64_t ticket);
    ::jxx::lang::jbool eligible_(std::uint64_t ticket, ::jxx::lang::jint permits) const;

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::lang::jint permits_;
    ::jxx::lang::jbool fair_;
    std::deque<Waiter> waiters_;
    std::uint64_t nextTicket_ = 1U;
};

} // namespace jxx::util::concurrent
