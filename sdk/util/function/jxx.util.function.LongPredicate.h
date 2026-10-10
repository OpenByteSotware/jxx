#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class LongPredicate : public ::jxx::lang::InterfaceBase<LongPredicate> {
public:
    ~LongPredicate() override = default;
    virtual ::jxx::lang::jbool test(::jxx::lang::jlong value) = 0;
};
} // namespace jxx::util::function
