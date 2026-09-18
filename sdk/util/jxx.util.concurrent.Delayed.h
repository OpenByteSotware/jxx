#pragma once

#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

class Delayed
    : public ::jxx::lang::InterfaceBase<
          Delayed,
          ::jxx::lang::Comparable<Delayed>> {
public:
    using JxxSuper = ::jxx::lang::Comparable<Delayed>;
    using Super = ::jxx::lang::InterfaceBase<Delayed, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~Delayed() override = default;
    virtual ::jxx::lang::jlong getDelay(
        const ::jxx::Ptr<TimeUnit>& unit) const = 0;
};

} // namespace jxx::util::concurrent
