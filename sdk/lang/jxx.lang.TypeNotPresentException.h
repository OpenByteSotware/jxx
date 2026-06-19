#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class TypeNotPresentException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        JXX_OBJECT_CLONE(TypeNotPresentException)
    protected:
        const char* typeName() const noexcept override { return "TypeNotPresentException"; }
    };

} // namespace jxx::lang
