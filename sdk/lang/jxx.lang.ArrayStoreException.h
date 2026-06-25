#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class ArrayStoreException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(ArrayStoreException)
        const char* typeName() const noexcept override { return "ArrayStoreException"; }
    };

} // namespace jxx::lang
