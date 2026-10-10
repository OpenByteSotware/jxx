#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename R>
class LongFunction : public ::jxx::lang::InterfaceBase<LongFunction<R>> {
public:
    ~LongFunction() override = default;
    virtual ::jxx::Ptr<R> apply(::jxx::lang::jlong value) = 0;
};

} // namespace jxx::util::function
