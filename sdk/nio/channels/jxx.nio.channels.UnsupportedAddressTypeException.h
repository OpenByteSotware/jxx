#pragma once

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class UnsupportedAddressTypeException
    : public ::jxx::lang::ClassBase<UnsupportedAddressTypeException, ::jxx::lang::IllegalArgumentException> {
public:
    using JxxSuper = ::jxx::lang::IllegalArgumentException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<UnsupportedAddressTypeException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    UnsupportedAddressTypeException() = default;
    ~UnsupportedAddressTypeException() override = default;

protected:
    JXX_OBJECT_CLONE(UnsupportedAddressTypeException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
