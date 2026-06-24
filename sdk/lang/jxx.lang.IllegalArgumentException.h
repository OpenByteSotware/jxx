#pragma once

#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"

namespace jxx {
namespace lang {

    class IllegalArgumentException : public RuntimeException {
    public:
        using jxx::lang::RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(IllegalArgumentException)
        const char* typeName() const noexcept override { return "IllegalArgumentException"; }
    };

} // namespace lang
} // namespace jxx