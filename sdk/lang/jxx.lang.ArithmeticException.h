#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ArithmeticException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    
protected:
    JXX_OBJECT_CLONE(ArithmeticException)
    const char* typeName() const noexcept override { return "ArithmeticException"; }
};

} // namespace jxx::lang