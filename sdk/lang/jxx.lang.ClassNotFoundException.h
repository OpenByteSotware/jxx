#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

    class ClassNotFoundException : public jxx::lang::ReflectiveOperationException {
public:
    using JxxSuper = jxx::lang::ReflectiveOperationException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ClassNotFoundException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ClassNotFoundException() = default;

    ClassNotFoundException(const ClassNotFoundException&) = default;
    ClassNotFoundException(ClassNotFoundException&&) noexcept = default;
    ClassNotFoundException& operator=(const ClassNotFoundException&) = default;
    ClassNotFoundException& operator=(ClassNotFoundException&&) noexcept = default;
    ~ClassNotFoundException() override = default;

    public:
        using ReflectiveOperationException::ReflectiveOperationException;
        
    protected:
        JXX_OBJECT_CLONE(ClassNotFoundException)
        const char* typeName() const noexcept override { return "ClassNotFoundException"; }
    };

} // namespace jxx::lang