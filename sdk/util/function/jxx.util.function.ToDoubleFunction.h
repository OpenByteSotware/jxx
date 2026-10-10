#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T>
class ToDoubleFunction : public ::jxx::lang::InterfaceBase<ToDoubleFunction<T>> {
public:
    ~ToDoubleFunction() override = default;
    virtual ::jxx::lang::jdouble applyAsDouble(const ::jxx::Ptr<T>& value) = 0;
};

} // namespace jxx::util::function
