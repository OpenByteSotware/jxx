#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class NegativeArraySizeException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        JXX_OBJECT_CLONE(NegativeArraySizeException)
    protected:
        const char* typeName() const noexcept override { return "NegativeArraySizeException"; }
    };

} // namespace jxx::lang
