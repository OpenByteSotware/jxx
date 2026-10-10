#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class IntToLongFunction : public ::jxx::lang::InterfaceBase<IntToLongFunction> {
public:
    ~IntToLongFunction() override = default;
    virtual ::jxx::lang::jlong applyAsLong(::jxx::lang::jint value) = 0;
};

} // namespace jxx::util::function
