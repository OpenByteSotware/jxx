#pragma once

#include <memory>
#include "lang/jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

    class ClassNotFoundException : public jxx::lang::ReflectiveOperationException {
    public:
        using ReflectiveOperationException::ReflectiveOperationException;
        JXX_OBJECT_CLONE(jxx::lang::ClassNotFoundException)
    protected:
        const char* typeName() const noexcept override { return "ClassNotFoundException"; }
    };

} // namespace jxx::lang