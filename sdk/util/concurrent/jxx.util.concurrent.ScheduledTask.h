#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

#include <functional>
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent::scheduled_detail {

class ScheduledTask
    : public ::jxx::lang::InterfaceBase<ScheduledTask> {
public:
    ~ScheduledTask() override = default;
    virtual void run() = 0;
    virtual ::jxx::lang::jlong getDelay(
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) const = 0;
    virtual ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) = 0;
    virtual ::jxx::lang::jbool isCancelled() = 0;
    virtual ::jxx::lang::jbool isDone() = 0;
    virtual ::jxx::lang::jbool isPeriodic() const noexcept = 0;
    virtual ::jxx::lang::jlong scheduledTimeNanos() const noexcept = 0;
    virtual ::jxx::lang::jlong sequenceNumber() const noexcept = 0;
    virtual void setCancelCallback(std::function<void()> callback) = 0;
};

} // namespace jxx::util::concurrent::scheduled_detail
