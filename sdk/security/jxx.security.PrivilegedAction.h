#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::security {

template<typename T>
class PrivilegedAction
    : public ::jxx::lang::InterfaceBase<PrivilegedAction<T>> {
public:
    using Super = ::jxx::lang::InterfaceBase<PrivilegedAction<T>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~PrivilegedAction() override = default;

    virtual ::jxx::Ptr<T> run() = 0;
};

} // namespace jxx::security
