#pragma once
#include "lang/jxx.lang.ClassFormatError.h"
namespace jxx::lang {
class UnsupportedClassVersionError : public ClassFormatError {
public:
	using ClassFormatError::ClassFormatError;
protected:
    JXX_OBJECT_CLONE(UnsupportedClassVersionError)

    const char* typeName() const noexcept override { return "UnsupportedClassVersionError"; }
};
}
