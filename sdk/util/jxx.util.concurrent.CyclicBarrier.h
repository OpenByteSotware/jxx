#pragma once

#include <condition_variable>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class CyclicBarrier final
    : public ::jxx::lang::ClassBase<CyclicBarrier, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<CyclicBarrier, JxxSuper>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<CyclicBarrier, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    explicit CyclicBarrier(::jxx::lang::jint parties);
    CyclicBarrier(::jxx::lang::jint parties,
        const ::jxx::Ptr<::jxx::lang::Runnable>& barrierAction);

    ::jxx::lang::jint await();
    ::jxx::lang::jint await(::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit);
    ::jxx::lang::jint getParties() const noexcept;
    ::jxx::lang::jint getNumberWaiting() const;
    ::jxx::lang::jbool isBroken() const;
    void reset();

private:
    ::jxx::lang::jint wait_(const std::chrono::nanoseconds* timeout);
    void nextGeneration_();
    void breakBarrier_();

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    const ::jxx::lang::jint parties_;
    ::jxx::lang::jint count_;
    ::jxx::lang::jlong generation_ = 0;
    ::jxx::lang::jbool broken_ = false;
    ::jxx::Ptr<::jxx::lang::Runnable> barrierAction_;
};

} // namespace jxx::util::concurrent
