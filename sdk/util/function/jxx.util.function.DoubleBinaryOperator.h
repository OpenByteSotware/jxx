#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class DoubleBinaryOperator : public ::jxx::lang::InterfaceBase<DoubleBinaryOperator> {
public:
 using Super=::jxx::lang::InterfaceBase<DoubleBinaryOperator>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~DoubleBinaryOperator() override=default;
 virtual ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jdouble left,::jxx::lang::jdouble right)=0;
};}
