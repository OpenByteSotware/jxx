#pragma once

#include <memory>
#include "lang/jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

    class ClassNotFoundException : public jxx::lang::ReflectiveOperationException {
    public:
        using ReflectiveOperationException::ReflectiveOperationException;
        
    protected:
        JXX_OBJECT_CLONE(ClassNotFoundException)
        const char* typeName() const noexcept override { return "ClassNotFoundException"; }
    };

} // namespace jxx::lang