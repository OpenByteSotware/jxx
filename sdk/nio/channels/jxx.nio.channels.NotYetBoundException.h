#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx::nio::channels {

class NotYetBoundException
    : public ::jxx::lang::ClassBase<
          NotYetBoundException,
          ::jxx::lang::IllegalStateException> {
public:
    using JxxSuper = ::jxx::lang::IllegalStateException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<NotYetBoundException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    NotYetBoundException() = default;
    ~NotYetBoundException() override = default;

protected:
    JXX_OBJECT_CLONE(NotYetBoundException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::nio::channels
