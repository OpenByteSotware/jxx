#pragma once
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class NoSuchMethodError : public IncompatibleClassChangeError {
public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(NoSuchMethodError)

    const char* typeName() const noexcept override { return "NoSuchMethodError"; }
};
}
