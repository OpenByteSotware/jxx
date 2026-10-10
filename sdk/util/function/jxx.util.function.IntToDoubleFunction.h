#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class IntToDoubleFunction : public ::jxx::lang::InterfaceBase<IntToDoubleFunction> {
public:
    ~IntToDoubleFunction() override = default;
    virtual ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jint value) = 0;
};

} // namespace jxx::util::function
