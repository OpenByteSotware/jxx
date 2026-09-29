#pragma once

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::nio::channels {

class AlreadyBoundException
    : public ::jxx::lang::ClassBase<AlreadyBoundException, ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<AlreadyBoundException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    AlreadyBoundException() = default;
    ~AlreadyBoundException() override = default;

protected:
    JXX_OBJECT_CLONE(AlreadyBoundException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
