#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class NullPointerException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(NullPointerException)
        const char* typeName() const noexcept override { return "NullPointerException"; }
    };

} // namespace jxx::lang
