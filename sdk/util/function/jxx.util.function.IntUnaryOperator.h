#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class IntUnaryOperator : public ::jxx::lang::InterfaceBase<IntUnaryOperator> {
public:
    ~IntUnaryOperator() override = default;
    virtual ::jxx::lang::jint applyAsInt(::jxx::lang::jint operand) = 0;
};
} // namespace jxx::util::function
