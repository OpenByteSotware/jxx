#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::security {

template<typename T>
class PrivilegedExceptionAction
    : public ::jxx::lang::InterfaceBase<PrivilegedExceptionAction<T>> {
public:
    using Super = ::jxx::lang::InterfaceBase<PrivilegedExceptionAction<T>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    ~PrivilegedExceptionAction() override = default;

    virtual ::jxx::Ptr<T> run() = 0;
};

} // namespace jxx::security
