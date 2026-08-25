#pragma once
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class AbstractMethodError : public IncompatibleClassChangeError {
public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(AbstractMethodError)

    const char* typeName() const noexcept override { return "AbstractMethodError"; }
};
}
