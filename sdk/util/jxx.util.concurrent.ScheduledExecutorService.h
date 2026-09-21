#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.ExecutorService.h"
#include "util/jxx.util.concurrent.ScheduledFuture.h"
#include "util/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class ScheduledExecutorService
    : public ::jxx::lang::InterfaceBase<
          ScheduledExecutorService,
          ExecutorService> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        ScheduledExecutorService, ExecutorService>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~ScheduledExecutorService() override = default;

    template<typename V>
    ::jxx::Ptr<ScheduledFuture<V>> schedule(
        const ::jxx::Ptr<Callable<V>>& callable,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<TimeUnit>& unit) {
        if (callable == nullptr || unit == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        auto task = ::jxx::NEW<ScheduledFutureTask<V>>(
            callable, 0);
        (void)schedule(
            ::jxx::CAST<::jxx::lang::Runnable>(task),
            delay,
            unit);
        return ::jxx::CAST<ScheduledFuture<V>>(task);
    }

    virtual ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> schedule(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;

    virtual ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> scheduleAtFixedRate(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong period,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;

    virtual ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> scheduleWithFixedDelay(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
};

} // namespace jxx::util::concurrent
