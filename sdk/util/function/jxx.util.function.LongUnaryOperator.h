#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class LongUnaryOperator : public ::jxx::lang::InterfaceBase<LongUnaryOperator> {
public:
    ~LongUnaryOperator() override = default;
    virtual ::jxx::lang::jlong applyAsLong(::jxx::lang::jlong operand) = 0;
};
} // namespace jxx::util::function
