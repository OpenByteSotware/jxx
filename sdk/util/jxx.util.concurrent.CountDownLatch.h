#pragma once

#include <condition_variable>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class CountDownLatch final
    : public ::jxx::lang::ClassBase<CountDownLatch, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<CountDownLatch, JxxSuper>;

    explicit CountDownLatch(::jxx::lang::jint count);

    void await();
    ::jxx::lang::jbool await(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit);
    void countDown();
    ::jxx::lang::jlong getCount() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::lang::jlong count_;
};

} // namespace jxx::util::concurrent
