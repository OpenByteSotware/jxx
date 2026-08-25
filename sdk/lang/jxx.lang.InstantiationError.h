#pragma once
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class InstantiationError : public IncompatibleClassChangeError {
public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(InstantiationError)

    const char* typeName() const noexcept override { return "InstantiationError"; }
};
}
