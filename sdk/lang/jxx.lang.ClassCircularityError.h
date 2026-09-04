#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class ClassCircularityError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ClassCircularityError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ClassCircularityError() = default;

    ClassCircularityError(const ClassCircularityError&) = default;
    ClassCircularityError(ClassCircularityError&&) noexcept = default;
    ClassCircularityError& operator=(const ClassCircularityError&) = default;
    ClassCircularityError& operator=(ClassCircularityError&&) noexcept = default;
    ~ClassCircularityError() override = default;

public:
	using LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(ClassCircularityError)

    const char* typeName() const noexcept override { return "ClassCircularityError"; }
};
}
