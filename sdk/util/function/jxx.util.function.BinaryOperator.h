#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/function/jxx.util.function.BiFunction.h"

namespace jxx::util::function {

template <typename T>
class BinaryOperator
    : public ::jxx::lang::InterfaceBase<
          BinaryOperator<T>,
          BiFunction<T, T, T>> {
public:
    ~BinaryOperator() override = default;

    virtual ::jxx::Ptr<T> apply(
        const ::jxx::Ptr<T>& left,
        const ::jxx::Ptr<T>& right) override = 0;
};

} // namespace jxx::util::function
