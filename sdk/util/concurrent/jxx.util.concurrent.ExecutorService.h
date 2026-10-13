#pragma once
#include <chrono>
#include <thread>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.ArrayList.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.Future.h"
#include "util/concurrent/jxx.util.concurrent.FutureTask.h"
#include "util/concurrent/jxx.util.concurrent.RunnableAdapter.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"
#include "util/concurrent/jxx.util.concurrent.Executor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "util/concurrent/jxx.util.concurrent.wildcard.FutureAny.h"
namespace jxx::util::concurrent {
/** Java 8 interface. Interfaces do not inherit Object in JXX. */
class ExecutorService : public ::jxx::lang::InterfaceBase<ExecutorService, Executor> {
public:
    virtual ~ExecutorService() = default;
    virtual void shutdown() = 0;
    virtual ::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> shutdownNow() = 0;
    virtual ::jxx::lang::jbool isShutdown() = 0;
    virtual ::jxx::lang::jbool isTerminated() = 0;
    virtual ::jxx::lang::jbool awaitTermination(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
    template <typename T>
    ::jxx::Ptr<Future<T>> submit(const ::jxx::Ptr<Callable<T>>& task) {
        if (task == nullptr) throw ::jxx::lang::NullPointerException();
        auto future = ::jxx::NEW<FutureTask<T>>(task);
        execute(::jxx::CAST<::jxx::lang::Runnable>(future));
        return ::jxx::CAST<Future<T>>(future);
    }

    template <typename T>
    ::jxx::Ptr<Future<T>> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<T>& result) {
        if (task == nullptr) throw ::jxx::lang::NullPointerException();
        auto callable = ::jxx::NEW<RunnableAdapter<T>>(task, result);
        return submit<T>(::jxx::CAST<Callable<T>>(callable));
    }

    template <typename T>
    ::jxx::Ptr<::jxx::util::List<Future<T>>> invokeAll(
        const ::jxx::Ptr<::jxx::util::List<Callable<T>>>& tasks) {
        if (tasks == nullptr) throw ::jxx::lang::NullPointerException();
        auto futures = ::jxx::NEW<::jxx::util::ArrayList<Future<T>>>(tasks->size());
        try {
            for (::jxx::lang::jint i = 0; i < tasks->size(); ++i) {
                const auto task = tasks->get(i);
                if (task == nullptr) throw ::jxx::lang::NullPointerException();
                futures->add(submit<T>(task));
            }
            for (::jxx::lang::jint i = 0; i < futures->size(); ++i) {
                if (!futures->get(i)->isDone()) (void)futures->get(i)->get();
            }
            return ::jxx::CAST<::jxx::util::List<Future<T>>>(futures);
        } catch (...) {
            cancelAll_(futures);
            throw;
        }
    }

    template <typename T>
    ::jxx::Ptr<::jxx::util::List<Future<T>>> invokeAll(
        const ::jxx::Ptr<::jxx::util::List<Callable<T>>>& tasks,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        if (tasks == nullptr || unit == nullptr) throw ::jxx::lang::NullPointerException();
        auto futures = ::jxx::NEW<::jxx::util::ArrayList<Future<T>>>(tasks->size());
        const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(timeout);
        try {
            for (::jxx::lang::jint i = 0; i < tasks->size(); ++i) {
                const auto task = tasks->get(i);
                if (task == nullptr) throw ::jxx::lang::NullPointerException();
                if (std::chrono::steady_clock::now() >= deadline) break;
                futures->add(submit<T>(task));
            }
            for (::jxx::lang::jint i = 0; i < futures->size(); ++i) {
                const auto remaining = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    deadline - std::chrono::steady_clock::now()).count();
                if (remaining <= 0) break;
                try { (void)futures->get(i)->get(remaining, TimeUnit::NANOSECONDS()); }
                catch (const TimeoutException&) { break; }
            }
            cancelIncomplete_(futures);
            return ::jxx::CAST<::jxx::util::List<Future<T>>>(futures);
        } catch (...) {
            cancelAll_(futures);
            throw;
        }
    }

    template <typename T>
    ::jxx::Ptr<T> invokeAny(
        const ::jxx::Ptr<::jxx::util::List<Callable<T>>>& tasks) {
        return invokeAnyImpl_<T>(tasks, false, 0, nullptr);
    }

    template <typename T>
    ::jxx::Ptr<T> invokeAny(
        const ::jxx::Ptr<::jxx::util::List<Callable<T>>>& tasks,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        return invokeAnyImpl_<T>(tasks, true, timeout, unit);
    }

    virtual ::jxx::Ptr<wildcard::FutureAny> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task) = 0;

private:
    template <typename T>
    static void cancelAll_(const ::jxx::Ptr<::jxx::util::ArrayList<Future<T>>>& futures) {
        for (::jxx::lang::jint i = 0; i < futures->size(); ++i)
            (void)futures->get(i)->cancel(true);
    }
    template <typename T>
    static void cancelIncomplete_(const ::jxx::Ptr<::jxx::util::ArrayList<Future<T>>>& futures) {
        for (::jxx::lang::jint i = 0; i < futures->size(); ++i)
            if (!futures->get(i)->isDone()) (void)futures->get(i)->cancel(true);
    }
    template <typename T>
    ::jxx::Ptr<T> invokeAnyImpl_(
        const ::jxx::Ptr<::jxx::util::List<Callable<T>>>& tasks,
        ::jxx::lang::jbool timed,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        if (tasks == nullptr) throw ::jxx::lang::NullPointerException();
        if (tasks->isEmpty()) throw ::jxx::lang::IllegalArgumentException();
        auto futures = ::jxx::NEW<::jxx::util::ArrayList<Future<T>>>(tasks->size());
        for (::jxx::lang::jint i = 0; i < tasks->size(); ++i) {
            const auto task = tasks->get(i);
            if (task == nullptr) { cancelAll_(futures); throw ::jxx::lang::NullPointerException(); }
            futures->add(submit<T>(task));
        }
        const auto deadline = timed
            ? std::chrono::steady_clock::now() + unit->toChrono(timeout)
            : std::chrono::steady_clock::time_point::max();
        ::jxx::lang::jbool sawFailure = false;
        for (;;) {
            ::jxx::lang::jbool allDone = true;
            for (::jxx::lang::jint i = 0; i < futures->size(); ++i) {
                const auto future = futures->get(i);
                if (!future->isDone()) { allDone = false; continue; }
                if (future->isCancelled()) continue;
                try {
                    auto value = future->get();
                    cancelAll_(futures);
                    return value;
                } catch (const ExecutionException&) { sawFailure = true; }
            }
            if (allDone) { cancelAll_(futures); throw ExecutionException(); }
            if (timed && std::chrono::steady_clock::now() >= deadline) {
                cancelAll_(futures); throw TimeoutException();
            }
            (void)sawFailure;
            std::this_thread::yield();
        }
    }

};
} // namespace jxx::util::concurrent
