#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.concurrent.Executor.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.wildcard.FutureAny.h"
namespace jxx::util::concurrent {
/** Java 8 interface. Interfaces do not inherit Object in JXX. */
class ExecutorService : public virtual Executor {
public:
    virtual ~ExecutorService() = default;
    virtual void shutdown() = 0;
    virtual jxx::Ptr<jxx::util::List<jxx::lang::Runnable>> shutdownNow() = 0;
    virtual jxx::lang::jbool isShutdown() = 0;
    virtual jxx::lang::jbool isTerminated() = 0;
    virtual jxx::lang::jbool awaitTermination(
        jxx::lang::jlong timeout,
        const jxx::Ptr<TimeUnit>& unit) = 0;
    virtual jxx::Ptr<wildcard::FutureAny> submit(
        const jxx::Ptr<jxx::lang::Runnable>& task) = 0;
};
} // namespace jxx::util::concurrent
