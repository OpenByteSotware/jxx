#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.Date.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent::locks {

class Condition : public ::jxx::lang::InterfaceBase<Condition> {
public:
    using Super = ::jxx::lang::InterfaceBase<Condition>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    ~Condition() override = default;

    virtual void await() = 0;
    virtual void awaitUninterruptibly() = 0;
    virtual ::jxx::lang::jlong awaitNanos(
        ::jxx::lang::jlong nanosTimeout) = 0;
    virtual ::jxx::lang::jbool await(
        ::jxx::lang::jlong time,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) = 0;
    virtual ::jxx::lang::jbool awaitUntil(
        const ::jxx::Ptr<::jxx::util::Date>& deadline) = 0;
    virtual void signal() = 0;
    virtual void signalAll() = 0;
};

} // namespace jxx::util::concurrent::locks
