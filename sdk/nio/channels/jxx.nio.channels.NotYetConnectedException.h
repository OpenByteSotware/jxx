#pragma once

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class NotYetConnectedException
    : public ::jxx::lang::ClassBase<NotYetConnectedException, ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<NotYetConnectedException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    NotYetConnectedException() = default;
    ~NotYetConnectedException() override = default;

protected:
    JXX_OBJECT_CLONE(NotYetConnectedException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
