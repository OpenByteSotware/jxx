#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

    class EmptyStackException : public jxx::lang::RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        JXX_OBJECT_CLONE(EmptyStackException)
    protected:
        const char* typeName() const noexcept override { return "EmptyStackException"; }
    };

} // namespace jxx::util
