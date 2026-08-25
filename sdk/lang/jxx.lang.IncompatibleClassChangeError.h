#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class IncompatibleClassChangeError : public LinkageError {
public:
	using jxx::lang::LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(IncompatibleClassChangeError)

    const char* typeName() const noexcept override { return "IncompatibleClassChangeError"; }
};
}
