#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.ClassFormatError.h"
namespace jxx::lang {
class UnsupportedClassVersionError : public ClassFormatError {
public:
    using JxxSuper = ClassFormatError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<UnsupportedClassVersionError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    UnsupportedClassVersionError() = default;

    UnsupportedClassVersionError(const UnsupportedClassVersionError&) = default;
    UnsupportedClassVersionError(UnsupportedClassVersionError&&) noexcept = default;
    UnsupportedClassVersionError& operator=(const UnsupportedClassVersionError&) = default;
    UnsupportedClassVersionError& operator=(UnsupportedClassVersionError&&) noexcept = default;
    ~UnsupportedClassVersionError() override = default;

public:
	using ClassFormatError::ClassFormatError;
protected:
    JXX_OBJECT_CLONE(UnsupportedClassVersionError)

    const char* typeName() const noexcept override { return "UnsupportedClassVersionError"; }
};
}
