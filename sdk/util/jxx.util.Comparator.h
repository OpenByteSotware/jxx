#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::util {

template<typename E>
class Comparator
    : public ::jxx::lang::InterfaceBase<Comparator<E>> {
public:
    ~Comparator() override = default;
    virtual ::jxx::lang::jint compare(
        const ::jxx::Ptr<E>& first,
        const ::jxx::Ptr<E>& second) const = 0;
};

} // namespace jxx::util
