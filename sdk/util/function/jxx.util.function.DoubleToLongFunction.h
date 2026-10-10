#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class DoubleToLongFunction : public ::jxx::lang::InterfaceBase<DoubleToLongFunction> {
public:
    ~DoubleToLongFunction() override = default;
    virtual ::jxx::lang::jlong applyAsLong(::jxx::lang::jdouble value) = 0;
};

} // namespace jxx::util::function
