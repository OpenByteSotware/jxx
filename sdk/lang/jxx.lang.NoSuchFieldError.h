#pragma once
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class NoSuchFieldError : public IncompatibleClassChangeError {
public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(NoSuchFieldError)

    const char* typeName() const noexcept override { return "NoSuchFieldError"; }
};
}
