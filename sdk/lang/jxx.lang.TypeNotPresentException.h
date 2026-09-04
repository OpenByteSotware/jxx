#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class TypeNotPresentException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<TypeNotPresentException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    TypeNotPresentException() = default;

    TypeNotPresentException(const TypeNotPresentException&) = default;
    TypeNotPresentException(TypeNotPresentException&&) noexcept = default;
    TypeNotPresentException& operator=(const TypeNotPresentException&) = default;
    TypeNotPresentException& operator=(TypeNotPresentException&&) noexcept = default;
    ~TypeNotPresentException() override = default;

    public:
        using RuntimeException::RuntimeException;
        JXX_OBJECT_CLONE(TypeNotPresentException)
    protected:
        const char* typeName() const noexcept override { return "TypeNotPresentException"; }
    };

} // namespace jxx::lang
