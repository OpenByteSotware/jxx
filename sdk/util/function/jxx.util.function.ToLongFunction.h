#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T>
class ToLongFunction : public ::jxx::lang::InterfaceBase<ToLongFunction<T>> {
public:
    ~ToLongFunction() override = default;
    virtual ::jxx::lang::jlong applyAsLong(const ::jxx::Ptr<T>& value) = 0;
};

} // namespace jxx::util::function
