#pragma once
#include "lang/jxx.lang.Error.h"
namespace jxx::lang {
class LinkageError : public Error {
public:
    using jxx::lang::Error::Error;
protected:
    JXX_OBJECT_CLONE(LinkageError)

    const char* typeName() const noexcept override { return "LinkageError"; }
};
}
