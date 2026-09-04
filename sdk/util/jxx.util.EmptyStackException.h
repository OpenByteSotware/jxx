#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

    class EmptyStackException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<EmptyStackException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    EmptyStackException() = default;

    EmptyStackException(const EmptyStackException&) = default;
    EmptyStackException(EmptyStackException&&) noexcept = default;
    EmptyStackException& operator=(const EmptyStackException&) = default;
    EmptyStackException& operator=(EmptyStackException&&) noexcept = default;
    ~EmptyStackException() override = default;

    public:
        using RuntimeException::RuntimeException;
        JXX_OBJECT_CLONE(EmptyStackException)
    protected:
        const char* typeName() const noexcept override { return "EmptyStackException"; }
    };

} // namespace jxx::util
