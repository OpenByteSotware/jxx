#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T>
class ToIntFunction : public ::jxx::lang::InterfaceBase<ToIntFunction<T>> {
public:
    ~ToIntFunction() override = default;
    virtual ::jxx::lang::jint applyAsInt(const ::jxx::Ptr<T>& value) = 0;
};

} // namespace jxx::util::function
