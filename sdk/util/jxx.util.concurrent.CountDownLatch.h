#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class CountDownLatch final
    : public ::jxx::lang::ClassBase<
          CountDownLatch,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<CountDownLatch, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<CountDownLatch, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    explicit CountDownLatch(::jxx::lang::jint count)
        : Super(), count_(count) {
        if (count < 0) throw ::jxx::lang::IllegalArgumentException();
    }

    void await() {
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ != 0) {
            condition_.wait_for(lock, std::chrono::milliseconds(10));
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
        }
    }

    ::jxx::lang::jbool await(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        if (!unit) throw ::jxx::lang::NullPointerException();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        const auto deadline = std::chrono::steady_clock::now() +
            unit->toChrono(timeout);
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ != 0) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= deadline) return false;
            condition_.wait_for(lock, std::min(
                deadline - now,
                std::chrono::steady_clock::duration(
                    std::chrono::milliseconds(10))));
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
        }
        return true;
    }

    void countDown() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ > 0 && --count_ == 0) {
            condition_.notify_all();
        }
    }

    ::jxx::lang::jlong getCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::lang::jlong count_;
};

} // namespace jxx::util::concurrent
