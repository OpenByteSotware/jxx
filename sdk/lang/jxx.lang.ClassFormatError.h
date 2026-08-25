#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class ClassFormatError : public LinkageError {
public:
	using LinkageError::LinkageError;    
protected:
    JXX_OBJECT_CLONE(ClassFormatError)

    const char* typeName() const noexcept override { return "ClassFormatError"; }
};
}
