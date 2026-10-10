#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class DoubleToIntFunction : public ::jxx::lang::InterfaceBase<DoubleToIntFunction> {
public:
    ~DoubleToIntFunction() override = default;
    virtual ::jxx::lang::jint applyAsInt(::jxx::lang::jdouble value) = 0;
};

} // namespace jxx::util::function
