#pragma once

#include <atomic>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.ExecutorService.h"
#include "util/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/jxx.util.concurrent.ThreadFactory.h"

namespace jxx::util::concurrent {
namespace executors_detail {

template <typename T>
class RunnableAdapter final
    : public ::jxx::lang::ClassBase<
          RunnableAdapter<T>, ::jxx::lang::Object, Callable<T>> {
public:
    RunnableAdapter(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<T>& result)
        : task_(task), result_(result) {
        if (task_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::Ptr<T> call() override {
        task_->run();
        return result_;
    }

private:
    ::jxx::Ptr<::jxx::lang::Runnable> task_;
    ::jxx::Ptr<T> result_;
};

class DefaultThreadFactory final : public ThreadFactory {
public:
    ::jxx::Ptr<::jxx::lang::Thread> newThread(
        const ::jxx::Ptr<::jxx::lang::Runnable>& runnable) override {
        if (runnable == nullptr) throw ::jxx::lang::NullPointerException();
        const auto number = nextNumber_.fetch_add(1);
        auto thread = ::jxx::NEW<::jxx::lang::Thread>(
            runnable,
            ::jxx::NEW<::jxx::lang::String>(
                "pool-thread-" + std::to_string(number)));
        thread->setDaemon(false);
        thread->setPriority(::jxx::lang::Thread::NORM_PRIORITY);
        return thread;
    }

private:
    inline static std::atomic<::jxx::lang::jlong> nextNumber_{1};
};

} // namespace executors_detail

class Executors final : public ::jxx::lang::Object {
private:
    Executors() = delete;

public:
    static ::jxx::Ptr<ExecutorService> newFixedThreadPool(
        ::jxx::lang::jint threadCount);
    static ::jxx::Ptr<ExecutorService> newSingleThreadExecutor();
    static ::jxx::Ptr<ExecutorService> newCachedThreadPool();
    static ::jxx::Ptr<ScheduledExecutorService> newScheduledThreadPool(
        ::jxx::lang::jint corePoolSize);
    static ::jxx::Ptr<ScheduledExecutorService>
    newSingleThreadScheduledExecutor();

    static ::jxx::Ptr<ThreadFactory> defaultThreadFactory() {
        static const auto factory =
            ::jxx::Ptr<ThreadFactory>(new executors_detail::DefaultThreadFactory());
        return factory;
    }

    static ::jxx::Ptr<Callable<::jxx::lang::Object>> callable(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task) {
        return ::jxx::CAST<Callable<::jxx::lang::Object>>(
            ::jxx::NEW<executors_detail::RunnableAdapter<::jxx::lang::Object>>(
                task, nullptr));
    }

    template <typename T>
    static ::jxx::Ptr<Callable<T>> callable(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<T>& result) {
        return ::jxx::CAST<Callable<T>>(
            ::jxx::NEW<executors_detail::RunnableAdapter<T>>(task, result));
    }

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
};

} // namespace jxx::util::concurrent
