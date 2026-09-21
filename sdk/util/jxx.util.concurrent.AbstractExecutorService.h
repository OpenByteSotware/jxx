#pragma once
#include <chrono>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.concurrent.ExecutorCompletionService.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.TimeoutException.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.ExecutorService.h"
#include "util/jxx.util.concurrent.FutureAnyTask.h"
#include "util/jxx.util.concurrent.FutureTask.h"
#include "util/jxx.util.concurrent.RunnableAdapter.h"
namespace jxx::util::concurrent {
/** Java class: Object -> AbstractExecutorService, implements ExecutorService. */
class AbstractExecutorService
    : public jxx::lang::Object
    , public virtual ExecutorService {
public:
    virtual ~AbstractExecutorService() = default;

    template <typename T>
    jxx::Ptr<Future<T>> submit(const jxx::Ptr<Callable<T>>& task) {
        if (task == nullptr) throw jxx::lang::NullPointerException();
        auto future = jxx::NEW<FutureTask<T>>(task);
        execute(jxx::CAST<jxx::lang::Runnable>(future));
        return jxx::CAST<Future<T>>(future);
    }

    template <typename T>
    jxx::Ptr<Future<T>> submit(
        const jxx::Ptr<jxx::lang::Runnable>& task,
        const jxx::Ptr<T>& result) {
        if (task == nullptr) throw jxx::lang::NullPointerException();
        auto callable = jxx::NEW<RunnableAdapter<T>>(task, result);
        return submit<T>(jxx::CAST<Callable<T>>(callable));
    }

    template <typename T>
    jxx::Ptr<jxx::util::List<Future<T>>> invokeAll(
        const jxx::Ptr<jxx::util::List<Callable<T>>>& tasks) {
        if (tasks == nullptr) throw jxx::lang::NullPointerException();
        auto futures = jxx::NEW<jxx::util::ArrayList<Future<T>>>(tasks->size());
        for (jxx::lang::jint index = 0; index < tasks->size(); ++index) {
            auto task = tasks->get(index);
            if (task == nullptr) throw jxx::lang::NullPointerException();
            futures->add(submit<T>(task));
        }
        try {
            for (jxx::lang::jint index = 0; index < futures->size(); ++index) {
                if (!futures->get(index)->isDone()) (void)futures->get(index)->get();
            }
        }
        catch (...) {
            for (jxx::lang::jint index = 0; index < futures->size(); ++index)
                (void)futures->get(index)->cancel(true);
            throw;
        }
        return jxx::CAST<jxx::util::List<Future<T>>>(futures);
    }

    template <typename T>
    jxx::Ptr<jxx::util::List<Future<T>>> invokeAll(
        const jxx::Ptr<jxx::util::List<Callable<T>>>& tasks,
        jxx::lang::jlong timeout,
        const jxx::Ptr<TimeUnit>& unit) {
        if (tasks == nullptr || unit == nullptr) throw jxx::lang::NullPointerException();
        auto futures = jxx::NEW<jxx::util::ArrayList<Future<T>>>(tasks->size());
        const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(timeout);
        for (jxx::lang::jint index = 0; index < tasks->size(); ++index) {
            auto task = tasks->get(index);
            if (task == nullptr) throw jxx::lang::NullPointerException();
            futures->add(submit<T>(task));
        }
        for (jxx::lang::jint index = 0; index < futures->size(); ++index) {
            auto future = futures->get(index);
            if (future->isDone()) continue;
            const auto remaining = std::chrono::duration_cast<std::chrono::nanoseconds>(
                deadline - std::chrono::steady_clock::now()).count();
            if (remaining <= 0) break;
            try { (void)future->get(remaining, TimeUnit::NANOSECONDS()); }
            catch (const TimeoutException&) { break; }
            catch (const ExecutionException&) {}
        }
        for (jxx::lang::jint index = 0; index < futures->size(); ++index)
            if (!futures->get(index)->isDone()) (void)futures->get(index)->cancel(true);
        return jxx::CAST<jxx::util::List<Future<T>>>(futures);
    }

    template <typename T>
    jxx::Ptr<T> invokeAny(const jxx::Ptr<jxx::util::List<Callable<T>>>& tasks) {
        if (tasks == nullptr) throw jxx::lang::NullPointerException();
        if (tasks->isEmpty()) throw jxx::lang::IllegalArgumentException();
        auto self = jxx::CAST<Executor>(thisPtr());
        auto completion = jxx::NEW<ExecutorCompletionService<T>>(self);
        auto futures = jxx::NEW<jxx::util::ArrayList<Future<T>>>();
        for (jxx::lang::jint index = 0; index < tasks->size(); ++index)
            futures->add(completion->submit(tasks->get(index)));
        ExecutionException lastFailure;
        for (jxx::lang::jint remaining = tasks->size(); remaining > 0; --remaining) {
            try {
                auto value = completion->take()->get();
                for (jxx::lang::jint i = 0; i < futures->size(); ++i) (void)futures->get(i)->cancel(true);
                return value;
            }
            catch (const ExecutionException& failure) { lastFailure = failure; }
        }
        throw lastFailure;
    }

    template <typename T>
    jxx::Ptr<T> invokeAny(
        const jxx::Ptr<jxx::util::List<Callable<T>>>& tasks,
        jxx::lang::jlong timeout,
        const jxx::Ptr<TimeUnit>& unit) {
        if (tasks == nullptr || unit == nullptr) throw jxx::lang::NullPointerException();
        if (tasks->isEmpty()) throw jxx::lang::IllegalArgumentException();
        auto completion = jxx::NEW<ExecutorCompletionService<T>>(jxx::CAST<Executor>(thisPtr()));
        auto futures = jxx::NEW<jxx::util::ArrayList<Future<T>>>();
        for (jxx::lang::jint index = 0; index < tasks->size(); ++index)
            futures->add(completion->submit(tasks->get(index)));
        const auto deadline = std::chrono::steady_clock::now() + unit->toChrono(timeout);
        for (;;) {
            const auto remaining = std::chrono::duration_cast<std::chrono::nanoseconds>(
                deadline - std::chrono::steady_clock::now()).count();
            if (remaining <= 0) break;
            auto completed = completion->poll(remaining, TimeUnit::NANOSECONDS());
            if (completed == nullptr) break;
            try {
                auto value = completed->get();
                for (jxx::lang::jint i = 0; i < futures->size(); ++i) (void)futures->get(i)->cancel(true);
                return value;
            }
            catch (const ExecutionException&) {}
        }
        for (jxx::lang::jint i = 0; i < futures->size(); ++i) (void)futures->get(i)->cancel(true);
        throw TimeoutException();
    }

    jxx::Ptr<wildcard::FutureAny> submit(
        const jxx::Ptr<jxx::lang::Runnable>& task) override {
        if (task == nullptr) throw jxx::lang::NullPointerException();
        auto callable = jxx::NEW<RunnableAdapter<jxx::lang::Object>>(task, nullptr);
        auto future = jxx::NEW<FutureAnyTask>(jxx::CAST<Callable<jxx::lang::Object>>(callable));
        execute(jxx::CAST<jxx::lang::Runnable>(future));
        return jxx::CAST<wildcard::FutureAny>(future);
    }
protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        throw jxx::lang::CloneNotSupportedException();
    }
};
} // namespace jxx::util::concurrent
