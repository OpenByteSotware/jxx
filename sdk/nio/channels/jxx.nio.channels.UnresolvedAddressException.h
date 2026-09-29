#pragma once

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class UnresolvedAddressException
    : public ::jxx::lang::ClassBase<UnresolvedAddressException, ::jxx::lang::IllegalArgumentException> {
public:
    using JxxSuper = ::jxx::lang::IllegalArgumentException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<UnresolvedAddressException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    UnresolvedAddressException() = default;
    ~UnresolvedAddressException() override = default;

protected:
    JXX_OBJECT_CLONE(UnresolvedAddressException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
