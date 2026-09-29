#pragma once

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class ConnectionPendingException
    : public ::jxx::lang::ClassBase<ConnectionPendingException, ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<ConnectionPendingException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    ConnectionPendingException() = default;
    ~ConnectionPendingException() override = default;

protected:
    JXX_OBJECT_CLONE(ConnectionPendingException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
