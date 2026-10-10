#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class LongToDoubleFunction : public ::jxx::lang::InterfaceBase<LongToDoubleFunction> {
public:
    ~LongToDoubleFunction() override = default;
    virtual ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jlong value) = 0;
};

} // namespace jxx::util::function
