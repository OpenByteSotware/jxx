#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class DoubleUnaryOperator : public ::jxx::lang::InterfaceBase<DoubleUnaryOperator> {
public:
    ~DoubleUnaryOperator() override = default;
    virtual ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jdouble operand) = 0;
};
} // namespace jxx::util::function
