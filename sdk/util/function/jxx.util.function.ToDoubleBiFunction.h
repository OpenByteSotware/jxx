#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T, typename U>
class ToDoubleBiFunction : public ::jxx::lang::InterfaceBase<ToDoubleBiFunction<T, U>> {
public:
    ~ToDoubleBiFunction() override = default;
    virtual ::jxx::lang::jdouble applyAsDouble(const ::jxx::Ptr<T>& left, const ::jxx::Ptr<U>& right) = 0;
};

} // namespace jxx::util::function
