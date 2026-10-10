#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T, typename U>
class ToLongBiFunction : public ::jxx::lang::InterfaceBase<ToLongBiFunction<T, U>> {
public:
    ~ToLongBiFunction() override = default;
    virtual ::jxx::lang::jlong applyAsLong(const ::jxx::Ptr<T>& left, const ::jxx::Ptr<U>& right) = 0;
};

} // namespace jxx::util::function
