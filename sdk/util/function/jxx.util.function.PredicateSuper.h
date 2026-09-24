#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class PredicateSuper : public ::jxx::lang::InterfaceBase<PredicateSuper<T>> {
public:
    virtual ~PredicateSuper() = default;
    virtual ::jxx::lang::jbool test(const ::jxx::Ptr<T>& value) = 0;
};

} // namespace function
} // namespace util
} // namespace jxx
