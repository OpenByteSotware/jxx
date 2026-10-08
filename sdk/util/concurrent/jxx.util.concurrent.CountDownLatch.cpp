#include <algorithm>
#include <chrono>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.CountDownLatch.h"

namespace jxx::util::concurrent {

CountDownLatch::CountDownLatch(::jxx::lang::jint count)
    : Super(), count_(count) {
    if (count < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

void CountDownLatch::await() {
    const auto current = ::jxx::lang::Thread::currentThread();
    if (::jxx::lang::Thread::interrupted()) {
        throw ::jxx::lang::InterruptedException();
    }

    current->setParkWakeup_([this] {
        condition_.notify_all();
    });

    try {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this, &current] {
            return count_ == 0 || current->isInterrupted();
        });

        current->clearParkWakeup_();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
    } catch (...) {
        current->clearParkWakeup_();
        throw;
    }
}

::jxx::lang::jbool CountDownLatch::await(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    if (unit == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto current = ::jxx::lang::Thread::currentThread();
    if (::jxx::lang::Thread::interrupted()) {
        throw ::jxx::lang::InterruptedException();
    }

    std::unique_lock<std::mutex> lock(mutex_);
    if (count_ == 0) {
        return true;
    }
    if (timeout <= 0) {
        return false;
    }

    const auto duration = unit->toChrono(timeout);
    const auto now = std::chrono::steady_clock::now();
    const auto maximum = std::chrono::steady_clock::time_point::max() - now;
    const auto deadline = duration >= maximum
        ? std::chrono::steady_clock::time_point::max()
        : now + duration;

    current->setParkWakeup_([this] {
        condition_.notify_all();
    });

    try {
        const bool completed = condition_.wait_until(
            lock,
            deadline,
            [this, &current] {
                return count_ == 0 || current->isInterrupted();
            });

        current->clearParkWakeup_();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        return completed && count_ == 0;
    } catch (...) {
        current->clearParkWakeup_();
        throw;
    }
}

void CountDownLatch::countDown() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (count_ > 0 && --count_ == 0) {
        condition_.notify_all();
    }
}

::jxx::lang::jlong CountDownLatch::getCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return count_;
}

} // namespace jxx::util::concurrent
