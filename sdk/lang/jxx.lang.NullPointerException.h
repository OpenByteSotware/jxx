#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class NullPointerException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        JXX_THROWABLE_CLONE(NullPointerException)
    protected:
        const char* typeName() const noexcept override { return "NullPointerException"; }
    };

} // namespace jxx::lang
