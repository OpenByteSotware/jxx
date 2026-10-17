#pragma once

#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.List.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.Future.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"

namespace jxx::ext::swing {

template <typename T, typename V>
class SwingWorker
    : public ::jxx::lang::ClassBase<
          SwingWorker<T, V>,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable,
          ::jxx::util::concurrent::Future<T>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        SwingWorker<T, V>,
        JxxSuper,
        ::jxx::lang::Runnable,
        ::jxx::util::concurrent::Future<T>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    enum class StateValue { PENDING, STARTED, DONE };

    SwingWorker() = default;
    ~SwingWorker() override = default;

    void execute() {
        ::jxx::Ptr<SwingWorker<T, V>> self;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (state_ != StateValue::PENDING || executionStarted_) {
                return;
            }
            executionStarted_ = true;
            state_ = StateValue::STARTED;
            self = ::jxx::CAST<SwingWorker<T, V>>(this->thisPtr());
        }
        std::thread([self] { self->run(); }).detach();
    }

    void run() override {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (completed_) {
                return;
            }
            executionStarted_ = true;
            state_ = StateValue::STARTED;
        }
        try {
            const auto value = doInBackground();
            finish_(value, nullptr);
        } catch (...) {
            finish_(nullptr, std::current_exception());
        }
    }

    ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) override {
        (void)mayInterruptIfRunning;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (completed_) {
                return false;
            }
            cancelled_ = true;
            completed_ = true;
            state_ = StateValue::DONE;
        }
        condition_.notify_all();
        done();
        return true;
    }

    ::jxx::lang::jbool isCancelled() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return cancelled_;
    }

    ::jxx::lang::jbool isDone() override {
        std::lock_guard<std::mutex> lock(mutex_);
        return completed_;
    }

    ::jxx::Ptr<T> get() override {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return completed_; });
        return report_();
    }

    ::jxx::Ptr<T> get(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override {
        if (unit == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        std::unique_lock<std::mutex> lock(mutex_);
        if (!completed_ &&
            !condition_.wait_for(
                lock,
                unit->toChrono(timeout),
                [this] { return completed_; })) {
            throw ::jxx::util::concurrent::TimeoutException();
        }
        return report_();
    }

    ::jxx::lang::jint getProgress() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return progress_;
    }

    StateValue getState() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_;
    }

protected:
    virtual ::jxx::Ptr<T> doInBackground() = 0;

    virtual void process(const ::jxx::Ptr<::jxx::util::List<V>>& chunks) {
        (void)chunks;
    }

    virtual void done() {
    }

    void publish(const std::vector<::jxx::Ptr<V>>& chunks) {
        auto list = ::jxx::NEW<::jxx::util::ArrayList<V>>();
        for (const auto& chunk : chunks) {
            list->add(chunk);
        }
        process(::jxx::CAST<::jxx::util::List<V>>(list));
    }

    template <typename... Values>
    void publish(const ::jxx::Ptr<V>& first, const Values&... remaining) {
        std::vector<::jxx::Ptr<V>> chunks{first, remaining...};
        publish(chunks);
    }

    void setProgress(::jxx::lang::jint progress) {
        if (progress < 0 || progress > 100) {
            throw ::jxx::lang::IllegalArgumentException();
        }
        std::lock_guard<std::mutex> lock(mutex_);
        progress_ = progress;
    }

private:
    void finish_(const ::jxx::Ptr<T>& value, std::exception_ptr failure) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (completed_) {
                return;
            }
            result_ = value;
            failure_ = failure;
            completed_ = true;
            state_ = StateValue::DONE;
        }
        condition_.notify_all();
        done();
    }

    ::jxx::Ptr<T> report_() {
        if (cancelled_) {
            throw ::jxx::util::concurrent::CancellationException();
        }
        if (failure_) {
            try {
                std::rethrow_exception(failure_);
            }
            catch (const ::jxx::lang::Throwable& failure) {
                throw ::jxx::util::concurrent::ExecutionException(
                    failure.cloneThrowable());
            }
            catch (...) {
                throw ::jxx::util::concurrent::ExecutionException();
            }
        }
        return result_;
    }

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    StateValue state_{StateValue::PENDING};
    ::jxx::lang::jint progress_{0};
    ::jxx::lang::jbool executionStarted_{false};
    ::jxx::lang::jbool completed_{false};
    ::jxx::lang::jbool cancelled_{false};
    ::jxx::Ptr<T> result_;
    std::exception_ptr failure_;
};

} // namespace jxx::ext::swing
