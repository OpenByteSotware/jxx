#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class IntBinaryOperator : public ::jxx::lang::InterfaceBase<IntBinaryOperator> {
public:
    ~IntBinaryOperator() override = default;
    virtual ::jxx::lang::jint applyAsInt(::jxx::lang::jint left, ::jxx::lang::jint right) = 0;
};
} // namespace jxx::util::function
