#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "util/jxx.util.concurrent.FutureAnyTask.h"

namespace jxx::util::concurrent {

FutureAnyTask::FutureAnyTask(
    const ::jxx::Ptr<Callable<::jxx::lang::Object>>& callable)
    : delegate_(::jxx::NEW<FutureTask<::jxx::lang::Object>>(callable)) {
}

void FutureAnyTask::run() { delegate_->run(); }
::jxx::lang::jbool FutureAnyTask::cancel(::jxx::lang::jbool value) { return delegate_->cancel(value); }
::jxx::lang::jbool FutureAnyTask::isCancelled() { return delegate_->isCancelled(); }
::jxx::lang::jbool FutureAnyTask::isDone() { return delegate_->isDone(); }
::jxx::Ptr<::jxx::lang::Object> FutureAnyTask::get() { return delegate_->get(); }
::jxx::Ptr<::jxx::lang::Object> FutureAnyTask::get(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    return delegate_->get(timeout, unit);
}
::jxx::Ptr<::jxx::lang::Object> FutureAnyTask::cloneImpl() const {
    throw ::jxx::lang::CloneNotSupportedException();
}

} // namespace jxx::util::concurrent
