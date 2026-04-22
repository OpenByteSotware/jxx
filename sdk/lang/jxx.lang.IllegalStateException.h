#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class IllegalStateException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        JXX_THROWABLE_CLONE(IllegalStateException)
    protected:
        const char* typeName() const noexcept override { return "IllegalStateException"; }
    };

} // namespace jxx::lang