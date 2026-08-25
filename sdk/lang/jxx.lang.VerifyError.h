#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class VerifyError : public LinkageError {
public:
	using LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(VerifyError)

    const char* typeName() const noexcept override { return "VerifyError"; }
};
}
