#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.FutureTask.h"
#include "util/jxx.util.concurrent.wildcard.FutureAny.h"
namespace jxx::util::concurrent {
class FutureAnyTask final
    : public jxx::lang::Object
    , public virtual jxx::lang::Runnable
    , public virtual wildcard::FutureAny {
private:
    jxx::Ptr<FutureTask<jxx::lang::Object>> delegate_;
public:
    explicit FutureAnyTask(const jxx::Ptr<Callable<jxx::lang::Object>>& callable)
        : delegate_(jxx::NEW<FutureTask<jxx::lang::Object>>(callable)) {}
    void run() override { delegate_->run(); }
    jxx::lang::jbool cancel(jxx::lang::jbool mayInterruptIfRunning) override { return delegate_->cancel(mayInterruptIfRunning); }
    jxx::lang::jbool isCancelled() override { return delegate_->isCancelled(); }
    jxx::lang::jbool isDone() override { return delegate_->isDone(); }
    jxx::Ptr<jxx::lang::Object> get() override { return delegate_->get(); }
    jxx::Ptr<jxx::lang::Object> get(jxx::lang::jlong timeout,const jxx::Ptr<TimeUnit>& unit) override { return delegate_->get(timeout,unit); }
protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override { throw jxx::lang::CloneNotSupportedException(); }
};
} // namespace jxx::util::concurrent
