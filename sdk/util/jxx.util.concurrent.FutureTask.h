#pragma once

#include <condition_variable>
#include <exception>
#include <mutex>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.RunnableFuture.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {

template <typename V>
class FutureTask
    : public jxx::lang::Object
    , public virtual RunnableFuture<V> {
private:
    enum class State {
        NEW,
        RUNNING,
        NORMAL,
        EXCEPTIONAL,
        CANCELLED
    };

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    State state_ = State::NEW;
    jxx::Ptr<Callable<V>> callable_;
    jxx::Ptr<V> result_;
    std::exception_ptr failure_;

public:
    explicit FutureTask(const jxx::Ptr<Callable<V>>& callable)
        : callable_(callable) {
        if (callable_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
    }

    jxx::lang::jbool cancel(
        jxx::lang::jbool mayInterruptIfRunning) override {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (state_ == State::NEW ||
                (state_ == State::RUNNING && mayInterruptIfRunning)) {
                state_ = State::CANCELLED;
                callable_.reset();
            }
            else {
                return false;
            }
        }
        condition_.notify_all();
        done();
        return true;
    }

    jxx::lang::jbool isCancelled() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_ == State::CANCELLED;
    }

    jxx::lang::jbool isDone() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return isTerminal_(state_);
    }

    void run() override {
        jxx::Ptr<Callable<V>> callable;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (state_ != State::NEW) {
                return;
            }
            state_ = State::RUNNING;
            callable = callable_;
        }

        try {
            const auto value = callable->call();
            set(value);
        }
        catch (...) {
            setException(std::current_exception());
        }
    }

    jxx::Ptr<V> get() override {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] { return isTerminal_(state_); });
        return report_();
    }

    jxx::Ptr<V> get(
        jxx::lang::jlong timeout,
        const jxx::Ptr<TimeUnit>& unit) override {
        if (unit == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        std::unique_lock<std::mutex> lock(mutex_);
        if (!condition_.wait_for(
                lock,
                unit->toChrono(timeout),
                [&] { return isTerminal_(state_); })) {
            throw TimeoutException();
        }
        return report_();
    }

protected:
    virtual void done() {
    }

    void set(const jxx::Ptr<V>& value) {
        jxx::lang::jbool completed = false;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (state_ == State::NEW || state_ == State::RUNNING) {
                result_ = value;
                callable_.reset();
                state_ = State::NORMAL;
                completed = true;
            }
        }
        if (completed) {
            condition_.notify_all();
            done();
        }
    }

    void setException(std::exception_ptr failure) {
        jxx::lang::jbool completed = false;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (state_ == State::NEW || state_ == State::RUNNING) {
                failure_ = failure;
                callable_.reset();
                state_ = State::EXCEPTIONAL;
                completed = true;
            }
        }
        if (completed) {
            condition_.notify_all();
            done();
        }
    }

    jxx::lang::jbool runAndReset() {
        jxx::Ptr<Callable<V>> callable;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (state_ != State::NEW) {
                return false;
            }
            state_ = State::RUNNING;
            callable = callable_;
        }

        try {
            (void)callable->call();
        }
        catch (...) {
            setException(std::current_exception());
            return false;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        if (state_ != State::RUNNING) {
            return false;
        }
        state_ = State::NEW;
        return true;
    }

    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        throw jxx::lang::CloneNotSupportedException();
    }

private:
    static jxx::lang::jbool isTerminal_(State state) noexcept {
        return state == State::NORMAL ||
               state == State::EXCEPTIONAL ||
               state == State::CANCELLED;
    }

    jxx::Ptr<V> report_() {
        if (state_ == State::CANCELLED) {
            throw CancellationException();
        }
        if (state_ == State::EXCEPTIONAL) {
            try {
                std::rethrow_exception(failure_);
            }
            catch (const jxx::lang::Throwable& failure) {
                throw ExecutionException(failure.cloneThrowable());
            }
            catch (...) {
                throw ExecutionException();
            }
        }
        return result_;
    }
};

} // namespace jxx::util::concurrent
