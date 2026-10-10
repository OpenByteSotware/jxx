#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename R>
class DoubleFunction : public ::jxx::lang::InterfaceBase<DoubleFunction<R>> {
public:
    ~DoubleFunction() override = default;
    virtual ::jxx::Ptr<R> apply(::jxx::lang::jdouble value) = 0;
};

} // namespace jxx::util::function
