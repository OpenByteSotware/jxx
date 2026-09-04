#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"

namespace jxx {
namespace lang {

    class IllegalArgumentException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalArgumentException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalArgumentException() = default;

    IllegalArgumentException(const IllegalArgumentException&) = default;
    IllegalArgumentException(IllegalArgumentException&&) noexcept = default;
    IllegalArgumentException& operator=(const IllegalArgumentException&) = default;
    IllegalArgumentException& operator=(IllegalArgumentException&&) noexcept = default;
    ~IllegalArgumentException() override = default;

    public:
        using jxx::lang::RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(IllegalArgumentException)
        const char* typeName() const noexcept override { return "IllegalArgumentException"; }
    };

} // namespace lang
} // namespace jxx