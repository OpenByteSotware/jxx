#pragma once

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class AlreadyConnectedException
    : public ::jxx::lang::ClassBase<AlreadyConnectedException, ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<AlreadyConnectedException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    AlreadyConnectedException() = default;
    ~AlreadyConnectedException() override = default;

protected:
    JXX_OBJECT_CLONE(AlreadyConnectedException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
