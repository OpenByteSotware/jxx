#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include <atomic>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx_types.h"
#include "security/jxx.security.PrivilegedAction.h"
#include "security/jxx.security.PrivilegedExceptionAction.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.ExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinPool.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ThreadFactory.h"

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

template<typename T, typename Action>
class PrivilegedActionAdapter final
    : public ::jxx::lang::ClassBase<
          PrivilegedActionAdapter<T, Action>, ::jxx::lang::Object, Callable<T>> {
public:
    explicit PrivilegedActionAdapter(const ::jxx::Ptr<Action>& action)
        : action_(action) {
        if (action_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::Ptr<T> call() override {
        return action_->run();
    }

private:
    ::jxx::Ptr<Action> action_;
};

template<typename T>
class PrivilegedCallableAdapter final
    : public ::jxx::lang::ClassBase<
          PrivilegedCallableAdapter<T>, ::jxx::lang::Object, Callable<T>> {
public:
    explicit PrivilegedCallableAdapter(const ::jxx::Ptr<Callable<T>>& task)
        : task_(task) {
        if (task_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    ::jxx::Ptr<T> call() override {
        return task_->call();
    }

private:
    ::jxx::Ptr<Callable<T>> task_;
};

class DefaultThreadFactory final
    : public ::jxx::lang::ClassBase<
          DefaultThreadFactory,
          ::jxx::lang::Object,
          ThreadFactory> {
public:
    DefaultThreadFactory()
        : poolNumber_(nextPoolNumber_.fetch_add(1)) {}

    ::jxx::Ptr<::jxx::lang::Thread> newThread(
        const ::jxx::Ptr<::jxx::lang::Runnable>& runnable) override {
        if (runnable == nullptr) throw ::jxx::lang::NullPointerException();
        const auto threadNumber = nextThreadNumber_.fetch_add(1);
        auto thread = ::jxx::NEW<::jxx::lang::Thread>(
            runnable,
            ::jxx::NEW<::jxx::lang::String>(
                "pool-" + std::to_string(poolNumber_) +
                "-thread-" + std::to_string(threadNumber)));
        thread->setDaemon(false);
        thread->setPriority(::jxx::lang::Thread::NORM_PRIORITY);
        return thread;
    }

private:
    inline static std::atomic<::jxx::lang::jlong> nextPoolNumber_{1};
    const ::jxx::lang::jlong poolNumber_;
    std::atomic<::jxx::lang::jlong> nextThreadNumber_{1};
};

} // namespace executors_detail

class Executors final : public ::jxx::lang::ClassBase<Executors, ::jxx::lang::Object> {
private:
    Executors() = delete;

public:
    static ::jxx::Ptr<ExecutorService> newFixedThreadPool(
        ::jxx::lang::jint threadCount);
    static ::jxx::Ptr<ExecutorService> newFixedThreadPool(
        ::jxx::lang::jint threadCount,
        const ::jxx::Ptr<ThreadFactory>& threadFactory);
    static ::jxx::Ptr<ExecutorService> newSingleThreadExecutor();
    static ::jxx::Ptr<ExecutorService> newSingleThreadExecutor(
        const ::jxx::Ptr<ThreadFactory>& threadFactory);
    static ::jxx::Ptr<ExecutorService> newWorkStealingPool();
    static ::jxx::Ptr<ExecutorService> newWorkStealingPool(
        ::jxx::lang::jint parallelism);
    static ::jxx::Ptr<ExecutorService> newCachedThreadPool();
    static ::jxx::Ptr<ExecutorService> newCachedThreadPool(
        const ::jxx::Ptr<ThreadFactory>& threadFactory);
    static ::jxx::Ptr<ScheduledExecutorService> newScheduledThreadPool(
        ::jxx::lang::jint corePoolSize);
    static ::jxx::Ptr<ScheduledExecutorService> newScheduledThreadPool(
        ::jxx::lang::jint corePoolSize,
        const ::jxx::Ptr<ThreadFactory>& threadFactory);
    static ::jxx::Ptr<ScheduledExecutorService>
    newSingleThreadScheduledExecutor();
    static ::jxx::Ptr<ScheduledExecutorService>
    newSingleThreadScheduledExecutor(
        const ::jxx::Ptr<ThreadFactory>& threadFactory);

    static ::jxx::Ptr<ExecutorService> unconfigurableExecutorService(
        const ::jxx::Ptr<ExecutorService>& executor);
    static ::jxx::Ptr<ScheduledExecutorService>
    unconfigurableScheduledExecutorService(
        const ::jxx::Ptr<ScheduledExecutorService>& executor);

    static ::jxx::Ptr<ThreadFactory> defaultThreadFactory() {
        return ::jxx::CAST<ThreadFactory>(
            ::jxx::NEW<executors_detail::DefaultThreadFactory>());
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

    template<typename T>
    static ::jxx::Ptr<Callable<T>> callable(
        const ::jxx::Ptr<::jxx::security::PrivilegedAction<T>>& action) {
        using Action = ::jxx::security::PrivilegedAction<T>;
        return ::jxx::CAST<Callable<T>>(
            ::jxx::NEW<executors_detail::PrivilegedActionAdapter<T, Action>>(action));
    }

    template<typename T>
    static ::jxx::Ptr<Callable<T>> callable(
        const ::jxx::Ptr<::jxx::security::PrivilegedExceptionAction<T>>& action) {
        using Action = ::jxx::security::PrivilegedExceptionAction<T>;
        return ::jxx::CAST<Callable<T>>(
            ::jxx::NEW<executors_detail::PrivilegedActionAdapter<T, Action>>(action));
    }

    template<typename T>
    static ::jxx::Ptr<Callable<T>> privilegedCallable(
        const ::jxx::Ptr<Callable<T>>& callableTask) {
        return ::jxx::CAST<Callable<T>>(
            ::jxx::NEW<executors_detail::PrivilegedCallableAdapter<T>>(callableTask));
    }

    template<typename T>
    static ::jxx::Ptr<Callable<T>> privilegedCallableUsingCurrentClassLoader(
        const ::jxx::Ptr<Callable<T>>& callableTask) {
        return ::jxx::CAST<Callable<T>>(
            ::jxx::NEW<executors_detail::PrivilegedCallableAdapter<T>>(callableTask));
    }

    static ::jxx::Ptr<ThreadFactory> privilegedThreadFactory() {
        return defaultThreadFactory();
    }

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
};

} // namespace jxx::util::concurrent
