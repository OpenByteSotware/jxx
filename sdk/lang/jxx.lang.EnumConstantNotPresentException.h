#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class EnumConstantNotPresentException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(EnumConstantNotPresentException)
        const char* typeName() const noexcept override { return "EnumConstantNotPresentException"; }
    };

} // namespace jxx::lang
