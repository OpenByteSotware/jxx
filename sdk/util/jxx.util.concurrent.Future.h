#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

template<typename V>
class Future
    : public ::jxx::lang::InterfaceBase<Future<V>> {
public:
    using Super = ::jxx::lang::InterfaceBase<Future<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~Future() override = default;
    virtual ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) = 0;
    virtual ::jxx::lang::jbool isCancelled() = 0;
    virtual ::jxx::lang::jbool isDone() = 0;
    virtual ::jxx::Ptr<V> get() = 0;
    virtual ::jxx::Ptr<V> get(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) = 0;
};

} // namespace jxx::util::concurrent
