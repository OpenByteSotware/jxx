#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.concurrent.Delayed.h"
#include "util/jxx.util.concurrent.Future.h"

namespace jxx::util::concurrent {

template<typename V>
class ScheduledFuture
    : public ::jxx::lang::InterfaceBase<
          ScheduledFuture<V>,
          Delayed,
          Future<V>> {
public:
    using Super = ::jxx::lang::InterfaceBase<
        ScheduledFuture<V>, Delayed, Future<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~ScheduledFuture() override = default;
};

} // namespace jxx::util::concurrent
