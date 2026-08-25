#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class UnsatisfiedLinkError : public LinkageError {
public:
	using jxx::lang::LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(UnsatisfiedLinkError)

    const char* typeName() const noexcept override { return "UnsatisfiedLinkError"; }
};
}
