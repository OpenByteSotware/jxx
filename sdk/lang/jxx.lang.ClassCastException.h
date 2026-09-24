#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ClassCastException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ClassCastException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ClassCastException() = default;

    ClassCastException(const ClassCastException&) = default;
    ClassCastException(ClassCastException&&) noexcept = default;
    ClassCastException& operator=(const ClassCastException&) = default;
    ClassCastException& operator=(ClassCastException&&) noexcept = default;
    ~ClassCastException() override = default;

public:

    explicit ClassCastException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit ClassCastException(const char* message);
    
protected:
    JXX_OBJECT_CLONE(ClassCastException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang