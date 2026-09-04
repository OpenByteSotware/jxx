#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

    class IllegalThreadStateException : public IllegalArgumentException {
public:
    using JxxSuper = IllegalArgumentException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalThreadStateException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalThreadStateException() = default;

    IllegalThreadStateException(const IllegalThreadStateException&) = default;
    IllegalThreadStateException(IllegalThreadStateException&&) noexcept = default;
    IllegalThreadStateException& operator=(const IllegalThreadStateException&) = default;
    IllegalThreadStateException& operator=(IllegalThreadStateException&&) noexcept = default;
    ~IllegalThreadStateException() override = default;

    public:
        using IllegalArgumentException::IllegalArgumentException;
        JXX_OBJECT_CLONE(IllegalThreadStateException)
    protected:
        const char* typeName() const noexcept override { return "IllegalThreadStateException"; }
    };

} // namespace jxx::lang
