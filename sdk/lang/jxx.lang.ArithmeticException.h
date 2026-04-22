#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ArithmeticException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(ArithmeticException)
protected:
    const char* typeName() const noexcept override { return "ArithmeticException"; }
};

} // namespace jxx::lang