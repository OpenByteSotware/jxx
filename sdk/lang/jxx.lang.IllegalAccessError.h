#pragma once
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class IllegalAccessError : public IncompatibleClassChangeError {
public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(IllegalAccessError)

    const char* typeName() const noexcept override { return "IllegalAccessError"; }
};
}
