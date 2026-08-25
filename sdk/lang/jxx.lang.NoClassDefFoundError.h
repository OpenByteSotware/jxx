#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class NoClassDefFoundError : public LinkageError {
public:
	using jxx::lang::LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(NoClassDefFoundError)

    const char* typeName() const noexcept override { return "NoClassDefFoundError"; }
};
}
