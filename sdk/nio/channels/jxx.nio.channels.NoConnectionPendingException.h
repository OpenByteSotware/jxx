#pragma once

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class NoConnectionPendingException
    : public ::jxx::lang::ClassBase<NoConnectionPendingException, ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<NoConnectionPendingException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    NoConnectionPendingException() = default;
    ~NoConnectionPendingException() override = default;

protected:
    JXX_OBJECT_CLONE(NoConnectionPendingException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
