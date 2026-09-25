#include <algorithm>
#include <chrono>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.CountDownLatch.h"

namespace jxx::util::concurrent {

CountDownLatch::CountDownLatch(::jxx::lang::jint count)
    : Super(), count_(count) {
    if (count < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

void CountDownLatch::await() {
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

::jxx::lang::jbool CountDownLatch::await(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    if (unit == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
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
    while (count_ != 0) {
        const auto current = std::chrono::steady_clock::now();
        if (current >= deadline) {
            return false;
        }
        condition_.wait_for(
            lock,
            std::min(
                deadline - current,
                std::chrono::steady_clock::duration(
                    std::chrono::milliseconds(10))));
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
    }
    return true;
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
