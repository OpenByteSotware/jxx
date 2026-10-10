#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class LongToIntFunction : public ::jxx::lang::InterfaceBase<LongToIntFunction> {
public:
    ~LongToIntFunction() override = default;
    virtual ::jxx::lang::jint applyAsInt(::jxx::lang::jlong value) = 0;
};

} // namespace jxx::util::function
