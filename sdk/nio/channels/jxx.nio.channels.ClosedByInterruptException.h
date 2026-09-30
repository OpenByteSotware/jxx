#pragma once

#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"

namespace jxx::nio::channels {

class ClosedByInterruptException final
    : public ::jxx::lang::ClassBase<
          ClosedByInterruptException,
          AsynchronousCloseException> {
public:
    using JxxSuper = AsynchronousCloseException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<ClosedByInterruptException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    ClosedByInterruptException() = default;
    ~ClosedByInterruptException() override = default;

protected:
    JXX_OBJECT_CLONE(ClosedByInterruptException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
