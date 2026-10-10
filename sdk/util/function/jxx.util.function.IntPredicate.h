#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class IntPredicate : public ::jxx::lang::InterfaceBase<IntPredicate> {
public:
    ~IntPredicate() override = default;
    virtual ::jxx::lang::jbool test(::jxx::lang::jint value) = 0;
};
} // namespace jxx::util::function
