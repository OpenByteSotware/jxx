#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class IllegalStateException : public jxx::lang::RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(IllegalStateException)
        const char* typeName() const noexcept override { return "IllegalStateException"; }
    };

} // namespace jxx::lang