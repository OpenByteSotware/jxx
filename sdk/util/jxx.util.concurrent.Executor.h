#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Runnable.h"

namespace jxx::util::concurrent {

class Executor
    : public ::jxx::lang::InterfaceBase<Executor> {
public:
    using Super = ::jxx::lang::InterfaceBase<Executor>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~Executor() override = default;
    virtual void execute(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command) = 0;
};

} // namespace jxx::util::concurrent
