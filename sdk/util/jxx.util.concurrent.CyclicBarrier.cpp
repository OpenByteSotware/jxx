#include "util/jxx.util.concurrent.CyclicBarrier.h"

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.BrokenBarrierException.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {

::jxx::Ptr<::jxx::lang::ClassAny> CyclicBarrier::Class() {
    return JxxClassInfoMarker::Class();
}

CyclicBarrier::CyclicBarrier(::jxx::lang::jint parties)
    : CyclicBarrier(parties, nullptr) {}

CyclicBarrier::CyclicBarrier(::jxx::lang::jint parties,
    const ::jxx::Ptr<::jxx::lang::Runnable>& barrierAction)
    : Super(), parties_(parties), count_(parties), barrierAction_(barrierAction) {
    if (parties <= 0) throw ::jxx::lang::IllegalArgumentException();
}

::jxx::lang::jint CyclicBarrier::await() { return wait_(nullptr); }

::jxx::lang::jint CyclicBarrier::await(::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    if (unit == nullptr) throw ::jxx::lang::NullPointerException();
    const auto duration = unit->toChrono(timeout);
    return wait_(&duration);
}

::jxx::lang::jint CyclicBarrier::wait_(const std::chrono::nanoseconds* timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (broken_) throw BrokenBarrierException();
    const auto generation = generation_;
    const auto index = --count_;
    if (index == 0) {
        try {
            if (barrierAction_ != nullptr) barrierAction_->run();
            nextGeneration_();
            return 0;
        } catch (...) {
            breakBarrier_();
            throw;
        }
    }
    const auto advanced = [&] { return generation_ != generation || broken_; };
    if (timeout == nullptr) {
        condition_.wait(lock, advanced);
    } else if (!condition_.wait_for(lock, *timeout, advanced)) {
        breakBarrier_();
        throw TimeoutException();
    }
    if (broken_) throw BrokenBarrierException();
    return index;
}

void CyclicBarrier::nextGeneration_() {
    condition_.notify_all();
    count_ = parties_;
    ++generation_;
    broken_ = false;
}

void CyclicBarrier::breakBarrier_() {
    broken_ = true;
    count_ = parties_;
    condition_.notify_all();
}

::jxx::lang::jint CyclicBarrier::getParties() const noexcept { return parties_; }
::jxx::lang::jint CyclicBarrier::getNumberWaiting() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return parties_ - count_;
}
::jxx::lang::jbool CyclicBarrier::isBroken() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return broken_;
}
void CyclicBarrier::reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    breakBarrier_();
    ++generation_;
    broken_ = false;
}

} // namespace jxx::util::concurrent
