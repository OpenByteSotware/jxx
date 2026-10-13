#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::util::concurrent::internal {

class CancellableTask
    : public ::jxx::lang::InterfaceBase<CancellableTask> {
public:
    using Super = ::jxx::lang::InterfaceBase<CancellableTask>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~CancellableTask() override = default;

    virtual ::jxx::lang::jbool isCancelled() = 0;
};

} // namespace jxx::util::concurrent::internal
