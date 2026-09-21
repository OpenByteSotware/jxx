#include "util/jxx.util.Timer.h"

#include <atomic>
#include <algorithm>
#include <chrono>

namespace jxx::util {
namespace {
std::atomic<::jxx::lang::jlong> nextTimerNumber{0};
}

::jxx::lang::jlong Timer::now_() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

bool Timer::Later::operator()(const ::jxx::Ptr<TimerTask>& left,
                              const ::jxx::Ptr<TimerTask>& right) const {
    if (left.get() == right.get()) return false;
    std::scoped_lock lock(left->mutex_, right->mutex_);
    return left->nextExecutionTime_ > right->nextExecutionTime_;
}

Timer::Timer()
    : Timer(::jxx::NEW<::jxx::lang::String>(
          "Timer-" + std::to_string(nextTimerNumber.fetch_add(1))), false) {}
Timer::Timer(::jxx::lang::jbool daemon)
    : Timer(::jxx::NEW<::jxx::lang::String>(
          "Timer-" + std::to_string(nextTimerNumber.fetch_add(1))), daemon) {}
Timer::Timer(const ::jxx::Ptr<::jxx::lang::String>& name)
    : Timer(name, false) {}
Timer::Timer(const ::jxx::Ptr<::jxx::lang::String>& name,
             ::jxx::lang::jbool daemon)
    : Super(), daemon_(daemon) {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    name_ = name->utf8();
    workerThread_ = std::thread([this] { worker_(); });
}
Timer::~Timer() {
    cancel();
    if (workerThread_.joinable()) workerThread_.join();
}

void Timer::schedule(const ::jxx::Ptr<TimerTask>& task,
                     ::jxx::lang::jlong delay) {
    if (delay < 0) throw ::jxx::lang::IllegalArgumentException();
    schedule_(task, now_() + delay, 0, false);
}
void Timer::schedule(const ::jxx::Ptr<TimerTask>& task,
                     const ::jxx::Ptr<Date>& time) {
    if (time == nullptr) throw ::jxx::lang::NullPointerException();
    schedule_(task, std::max(now_(), time->getTime()), 0, false);
}
void Timer::schedule(const ::jxx::Ptr<TimerTask>& task,
                     ::jxx::lang::jlong delay,
                     ::jxx::lang::jlong period) {
    if (delay < 0 || period <= 0) throw ::jxx::lang::IllegalArgumentException();
    schedule_(task, now_() + delay, period, false);
}
void Timer::schedule(const ::jxx::Ptr<TimerTask>& task,
                     const ::jxx::Ptr<Date>& firstTime,
                     ::jxx::lang::jlong period) {
    if (firstTime == nullptr) throw ::jxx::lang::NullPointerException();
    if (period <= 0) throw ::jxx::lang::IllegalArgumentException();
    schedule_(task, std::max(now_(), firstTime->getTime()), period, false);
}
void Timer::scheduleAtFixedRate(const ::jxx::Ptr<TimerTask>& task,
                                ::jxx::lang::jlong delay,
                                ::jxx::lang::jlong period) {
    if (delay < 0 || period <= 0) throw ::jxx::lang::IllegalArgumentException();
    schedule_(task, now_() + delay, period, true);
}
void Timer::scheduleAtFixedRate(const ::jxx::Ptr<TimerTask>& task,
                                const ::jxx::Ptr<Date>& firstTime,
                                ::jxx::lang::jlong period) {
    if (firstTime == nullptr) throw ::jxx::lang::NullPointerException();
    if (period <= 0) throw ::jxx::lang::IllegalArgumentException();
    schedule_(task, std::max(now_(), firstTime->getTime()), period, true);
}

void Timer::schedule_(const ::jxx::Ptr<TimerTask>& task,
                      ::jxx::lang::jlong firstTime,
                      ::jxx::lang::jlong period,
                      ::jxx::lang::jbool fixedRate) {
    if (task == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> timerLock(mutex_);
    if (cancelled_) throw ::jxx::lang::IllegalStateException();
    {
        std::lock_guard<std::mutex> taskLock(task->mutex_);
        if (task->state_ != TimerTask::State::VIRGIN)
            throw ::jxx::lang::IllegalStateException();
        task->state_ = TimerTask::State::SCHEDULED;
        task->nextExecutionTime_ = firstTime;
        task->period_ = period;
        task->fixedRate_ = fixedRate;
    }
    queue_.push(task);
    changed_.notify_all();
}

void Timer::cancel() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (cancelled_) return;
    cancelled_ = true;
    while (!queue_.empty()) {
        const auto task = queue_.top(); queue_.pop();
        std::lock_guard<std::mutex> taskLock(task->mutex_);
        task->state_ = TimerTask::State::CANCELLED;
    }
    changed_.notify_all();
}

::jxx::lang::jint Timer::purge() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<::jxx::Ptr<TimerTask>> retained;
    ::jxx::lang::jint removed = 0;
    while (!queue_.empty()) {
        auto task = queue_.top(); queue_.pop();
        std::lock_guard<std::mutex> taskLock(task->mutex_);
        if (task->state_ == TimerTask::State::CANCELLED) ++removed;
        else retained.push_back(task);
    }
    for (const auto& task : retained) queue_.push(task);
    return removed;
}

void Timer::worker_() {
    std::unique_lock<std::mutex> lock(mutex_);
    for (;;) {
        if (cancelled_) return;
        if (queue_.empty()) {
            changed_.wait(lock, [this] { return cancelled_ || !queue_.empty(); });
            continue;
        }
        auto task = queue_.top();
        ::jxx::lang::jlong executionTime;
        {
            std::lock_guard<std::mutex> taskLock(task->mutex_);
            if (task->state_ == TimerTask::State::CANCELLED) {
                queue_.pop();
                continue;
            }
            executionTime = task->nextExecutionTime_;
        }
        const auto current = now_();
        if (executionTime > current) {
            changed_.wait_for(lock, std::chrono::milliseconds(executionTime-current));
            continue;
        }
        queue_.pop();
        ::jxx::lang::jlong period;
        ::jxx::lang::jbool fixedRate;
        {
            std::lock_guard<std::mutex> taskLock(task->mutex_);
            task->scheduledExecutionTime_ = executionTime;
            period = task->period_;
            fixedRate = task->fixedRate_;
            if (period == 0) task->state_ = TimerTask::State::EXECUTED;
        }
        lock.unlock();
        try { task->run(); }
        catch (...) {
            lock.lock();
            cancelled_ = true;
            changed_.notify_all();
            return;
        }
        lock.lock();
        if (period != 0) {
            std::lock_guard<std::mutex> taskLock(task->mutex_);
            if (task->state_ == TimerTask::State::SCHEDULED) {
                task->nextExecutionTime_ = fixedRate
                    ? executionTime + period
                    : now_() + period;
                queue_.push(task);
                changed_.notify_all();
            }
        }
    }
}

} // namespace jxx::util
