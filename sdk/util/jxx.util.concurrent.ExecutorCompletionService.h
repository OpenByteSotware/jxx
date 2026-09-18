#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.CompletionService.h"
#include "util/jxx.util.concurrent.Executor.h"
#include "util/jxx.util.concurrent.FutureTask.h"
#include "util/jxx.util.concurrent.LinkedBlockingQueue.h"
#include "util/jxx.util.concurrent.RunnableAdapter.h"

namespace jxx::util::concurrent {

template<typename V>
class ExecutorCompletionService final
    : public ::jxx::lang::ClassBase<ExecutorCompletionService<V>,
          ::jxx::lang::Object, CompletionService<V>> {
private:
    class QueueingFuture final
        : public ::jxx::lang::ClassBase<QueueingFuture,
              ::jxx::lang::Object, ::jxx::lang::Runnable> {
    public:
        using Super = ::jxx::lang::ClassBase<QueueingFuture,
            ::jxx::lang::Object, ::jxx::lang::Runnable>;
        QueueingFuture(const ::jxx::Ptr<FutureTask<V>>& task,
            const ::jxx::Ptr<BlockingQueue<Future<V>>>& completionQueue)
            : Super(), task_(task), completionQueue_(completionQueue) {}
        void run() override {
            task_->run();
            completionQueue_->add(::jxx::CAST<Future<V>>(task_));
        }
    private:
        ::jxx::Ptr<FutureTask<V>> task_;
        ::jxx::Ptr<BlockingQueue<Future<V>>> completionQueue_;
    };

public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ExecutorCompletionService<V>,
        JxxSuper, CompletionService<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() { return JxxClassInfoMarker::Class(); }

    explicit ExecutorCompletionService(const ::jxx::Ptr<Executor>& executor)
        : ExecutorCompletionService(executor,
              ::jxx::CAST<BlockingQueue<Future<V>>>(
                  ::jxx::NEW<LinkedBlockingQueue<Future<V>>>())) {}

    ExecutorCompletionService(const ::jxx::Ptr<Executor>& executor,
        const ::jxx::Ptr<BlockingQueue<Future<V>>>& completionQueue)
        : Super(), executor_(executor), completionQueue_(completionQueue) {
        if (executor_ == nullptr || completionQueue_ == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
    }

    ::jxx::Ptr<Future<V>> submit(const ::jxx::Ptr<Callable<V>>& task) override {
        if (task == nullptr) throw ::jxx::lang::NullPointerException();
        auto future = ::jxx::NEW<FutureTask<V>>(task);
        executor_->execute(::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<QueueingFuture>(future, completionQueue_)));
        return ::jxx::CAST<Future<V>>(future);
    }

    ::jxx::Ptr<Future<V>> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task,
        const ::jxx::Ptr<V>& result) override {
        if (task == nullptr) throw ::jxx::lang::NullPointerException();
        return submit(::jxx::CAST<Callable<V>>(
            ::jxx::NEW<RunnableAdapter<V>>(task, result)));
    }

    ::jxx::Ptr<Future<V>> take() override { return completionQueue_->take(); }
    ::jxx::Ptr<Future<V>> poll() override { return ::jxx::CAST<::jxx::util::Queue<Future<V>>>(completionQueue_)->poll(); }
    ::jxx::Ptr<Future<V>> poll(::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        return completionQueue_->poll(timeout, unit);
    }

private:
    ::jxx::Ptr<Executor> executor_;
    ::jxx::Ptr<BlockingQueue<Future<V>>> completionQueue_;
};

} // namespace jxx::util::concurrent
