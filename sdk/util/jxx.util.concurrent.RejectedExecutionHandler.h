#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
namespace jxx::util::concurrent {
class ThreadPoolExecutor;
/** Java 8 interface. */
class RejectedExecutionHandler {
public:
    virtual ~RejectedExecutionHandler() = default;
    virtual void rejectedExecution(
        const jxx::Ptr<jxx::lang::Runnable>& runnable,
        const jxx::Ptr<ThreadPoolExecutor>& executor) = 0;
};
} // namespace jxx::util::concurrent
