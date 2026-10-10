#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class DoublePredicate : public ::jxx::lang::InterfaceBase<DoublePredicate> {
public:
    ~DoublePredicate() override = default;
    virtual ::jxx::lang::jbool test(::jxx::lang::jdouble value) = 0;
};
} // namespace jxx::util::function
