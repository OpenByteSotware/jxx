#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::util::function {

template <typename T, typename U>
class BiPredicate
    : public ::jxx::lang::InterfaceBase<BiPredicate<T, U>> {
public:
    ~BiPredicate() override = default;

    virtual ::jxx::lang::jbool test(
        const ::jxx::Ptr<T>& left,
        const ::jxx::Ptr<U>& right) = 0;
};

} // namespace jxx::util::function
