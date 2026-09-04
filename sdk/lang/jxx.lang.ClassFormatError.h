#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class ClassFormatError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ClassFormatError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ClassFormatError() = default;

    ClassFormatError(const ClassFormatError&) = default;
    ClassFormatError(ClassFormatError&&) noexcept = default;
    ClassFormatError& operator=(const ClassFormatError&) = default;
    ClassFormatError& operator=(ClassFormatError&&) noexcept = default;
    ~ClassFormatError() override = default;

public:
	using LinkageError::LinkageError;    
protected:
    JXX_OBJECT_CLONE(ClassFormatError)

    const char* typeName() const noexcept override { return "ClassFormatError"; }
};
}
