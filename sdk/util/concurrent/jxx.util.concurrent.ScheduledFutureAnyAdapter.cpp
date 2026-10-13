#include "util/concurrent/jxx.util.concurrent.ScheduledFutureAnyAdapter.h"

namespace jxx::util::concurrent::scheduled_detail {

ScheduledFutureAnyAdapter::ScheduledFutureAnyAdapter(
    const ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<
        ::jxx::lang::Object>>& delegate)
    : delegate_(delegate) {
    if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::lang::jbool ScheduledFutureAnyAdapter::cancel(
    ::jxx::lang::jbool mayInterruptIfRunning) {
    return delegate_->cancel(mayInterruptIfRunning);
}

::jxx::lang::jbool ScheduledFutureAnyAdapter::isCancelled() {
    return delegate_->isCancelled();
}

::jxx::lang::jbool ScheduledFutureAnyAdapter::isDone() {
    return delegate_->isDone();
}

::jxx::Ptr<::jxx::lang::Object> ScheduledFutureAnyAdapter::get() {
    return delegate_->get();
}

::jxx::Ptr<::jxx::lang::Object> ScheduledFutureAnyAdapter::get(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
    return delegate_->get(timeout, unit);
}

::jxx::lang::jlong ScheduledFutureAnyAdapter::getDelay(
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) const {
    return delegate_->getDelay(unit);
}

::jxx::lang::jint ScheduledFutureAnyAdapter::compareTo(
    const ::jxx::Ptr<::jxx::util::concurrent::Delayed>& other) const {
    return delegate_->compareTo(other);
}

} // namespace jxx::util::concurrent::scheduled_detail
