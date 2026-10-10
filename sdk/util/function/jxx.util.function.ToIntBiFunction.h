#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T, typename U>
class ToIntBiFunction : public ::jxx::lang::InterfaceBase<ToIntBiFunction<T, U>> {
public:
    ~ToIntBiFunction() override = default;
    virtual ::jxx::lang::jint applyAsInt(const ::jxx::Ptr<T>& left, const ::jxx::Ptr<U>& right) = 0;
};

} // namespace jxx::util::function
