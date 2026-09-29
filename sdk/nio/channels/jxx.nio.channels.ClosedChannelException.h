#pragma once

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class ClosedChannelException
    : public ::jxx::lang::ClassBase<ClosedChannelException, ::jxx::io::IOException> {
public:
    using JxxSuper = ::jxx::io::IOException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<ClosedChannelException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    ClosedChannelException() = default;
    ~ClosedChannelException() override = default;

protected:
    JXX_OBJECT_CLONE(ClosedChannelException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
