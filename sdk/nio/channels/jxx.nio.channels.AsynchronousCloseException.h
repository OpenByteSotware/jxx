#pragma once

#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"

namespace jxx::nio::channels {

class AsynchronousCloseException
    : public ::jxx::lang::ClassBase<
          AsynchronousCloseException,
          ClosedChannelException> {
public:
    using JxxSuper = ClosedChannelException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<AsynchronousCloseException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    AsynchronousCloseException() = default;
    ~AsynchronousCloseException() override = default;

protected:
    JXX_OBJECT_CLONE(AsynchronousCloseException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
