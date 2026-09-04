#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Throwable.h"
namespace jxx::lang {
class Error : public Throwable {
public:
    using JxxSuper = Throwable;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<Error, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    Error() = default;

    Error(const Error&) = default;
    Error(Error&&) noexcept = default;
    Error& operator=(const Error&) = default;
    Error& operator=(Error&&) noexcept = default;
    ~Error() override = default;

public:
    using Throwable::Throwable;
    protected:
    JXX_OBJECT_CLONE(Error)

    const char* typeName() const noexcept override { return "Error"; }
};
}
