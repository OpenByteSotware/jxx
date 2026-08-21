#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
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
