#pragma once
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent::wildcard {
/** Erased Future<?> interface. Does not inherit Object. */
class FutureAny {
public:
    virtual ~FutureAny() = default;
    virtual jxx::lang::jbool cancel(jxx::lang::jbool mayInterruptIfRunning) = 0;
    virtual jxx::lang::jbool isCancelled() = 0;
    virtual jxx::lang::jbool isDone() = 0;
    virtual jxx::Ptr<jxx::lang::Object> get() = 0;
    virtual jxx::Ptr<jxx::lang::Object> get(
        jxx::lang::jlong timeout,
        const jxx::Ptr<TimeUnit>& unit) = 0;
};
} // namespace jxx::util::concurrent::wildcard
