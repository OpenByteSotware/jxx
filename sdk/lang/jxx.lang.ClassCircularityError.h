#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class ClassCircularityError : public LinkageError {
public:
	using LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(ClassCircularityError)

    const char* typeName() const noexcept override { return "ClassCircularityError"; }
};
}
