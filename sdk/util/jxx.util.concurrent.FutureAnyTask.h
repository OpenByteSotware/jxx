#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.FutureTask.h"
#include "util/jxx.util.concurrent.wildcard.FutureAny.h"

namespace jxx::util::concurrent {

class FutureAnyTask final
    : public ::jxx::lang::ClassBase<
          FutureAnyTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable,
          wildcard::FutureAny> {
public:
    explicit FutureAnyTask(
        const ::jxx::Ptr<Callable<::jxx::lang::Object>>& callable);

    void run() override;
    ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) override;
    ::jxx::lang::jbool isCancelled() override;
    ::jxx::lang::jbool isDone() override;
    ::jxx::Ptr<::jxx::lang::Object> get() override;
    ::jxx::Ptr<::jxx::lang::Object> get(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;

private:
    ::jxx::Ptr<FutureTask<::jxx::lang::Object>> delegate_;
};

} // namespace jxx::util::concurrent
