#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class BooleanSupplier : public ::jxx::lang::InterfaceBase<BooleanSupplier> {
public:
    ~BooleanSupplier() override = default;
    virtual ::jxx::lang::jbool getAsBoolean() = 0;
};
} // namespace jxx::util::function
