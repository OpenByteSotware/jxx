#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFuture.h"
#include "util/concurrent/jxx.util.concurrent.wildcard.FutureAny.h"

namespace jxx::util::concurrent::scheduled_detail {

class ScheduledFutureAnyAdapter final
    : public ::jxx::lang::ClassBase<
          ScheduledFutureAnyAdapter,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>,
          ::jxx::util::concurrent::wildcard::FutureAny> {
public:
    explicit ScheduledFutureAnyAdapter(
        const ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<
            ::jxx::lang::Object>>& delegate);

    ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) override;
    ::jxx::lang::jbool isCancelled() override;
    ::jxx::lang::jbool isDone() override;
    ::jxx::Ptr<::jxx::lang::Object> get() override;
    ::jxx::Ptr<::jxx::lang::Object> get(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override;
    ::jxx::lang::jlong getDelay(
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) const override;
    ::jxx::lang::jint compareTo(
        const ::jxx::Ptr<::jxx::util::concurrent::Delayed>& other) const override;

private:
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<
        ::jxx::lang::Object>> delegate_;
};

} // namespace jxx::util::concurrent::scheduled_detail
