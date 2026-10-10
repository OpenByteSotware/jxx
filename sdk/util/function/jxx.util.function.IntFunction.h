#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename R>
class IntFunction : public ::jxx::lang::InterfaceBase<IntFunction<R>> {
public:
    ~IntFunction() override = default;
    virtual ::jxx::Ptr<R> apply(::jxx::lang::jint value) = 0;
};

} // namespace jxx::util::function
