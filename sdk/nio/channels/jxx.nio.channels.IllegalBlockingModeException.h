#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx::nio::channels {

class IllegalBlockingModeException
    : public ::jxx::lang::ClassBase<
          IllegalBlockingModeException,
          ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<IllegalBlockingModeException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    IllegalBlockingModeException() = default;
    ~IllegalBlockingModeException() override = default;

protected:
    JXX_OBJECT_CLONE(IllegalBlockingModeException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
