#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent::scheduled_detail {

class ScheduledTask
    : public ::jxx::lang::InterfaceBase<
          ScheduledTask,
          ::jxx::lang::Runnable> {
public:
    ~ScheduledTask() override = default;
    virtual ::jxx::lang::jlong getDelay(
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) const = 0;
    virtual ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) = 0;
    virtual ::jxx::lang::jbool isCancelled() = 0;
    virtual ::jxx::lang::jbool isDone() = 0;
    virtual ::jxx::lang::jbool isPeriodic() const noexcept = 0;
};

} // namespace jxx::util::concurrent::scheduled_detail
