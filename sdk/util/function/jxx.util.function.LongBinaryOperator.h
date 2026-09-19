#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class LongBinaryOperator : public ::jxx::lang::InterfaceBase<LongBinaryOperator> {
public:
 using Super=::jxx::lang::InterfaceBase<LongBinaryOperator>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~LongBinaryOperator() override=default;
 virtual ::jxx::lang::jlong applyAsLong(::jxx::lang::jlong left,::jxx::lang::jlong right)=0;
};}
