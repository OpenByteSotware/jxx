#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class IllegalStateException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalStateException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalStateException() = default;

    IllegalStateException(const IllegalStateException&) = default;
    IllegalStateException(IllegalStateException&&) noexcept = default;
    IllegalStateException& operator=(const IllegalStateException&) = default;
    IllegalStateException& operator=(IllegalStateException&&) noexcept = default;
    ~IllegalStateException() override = default;

    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(IllegalStateException)
        const char* typeName() const noexcept override { return "IllegalStateException"; }
    };

} // namespace jxx::lang