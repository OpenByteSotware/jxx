#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.concurrent.RunnableFuture.h"
#include "util/jxx.util.concurrent.ScheduledFuture.h"

namespace jxx::util::concurrent {

template<typename V>
class RunnableScheduledFuture
    : public ::jxx::lang::InterfaceBase<
          RunnableScheduledFuture<V>,
          RunnableFuture<V>,
          ScheduledFuture<V>> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        RunnableScheduledFuture<V>, RunnableFuture<V>, ScheduledFuture<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~RunnableScheduledFuture() override = default;
    virtual ::jxx::lang::jbool isPeriodic() = 0;
};

} // namespace jxx::util::concurrent
