#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::nio {

class BufferUnderflowException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(BufferUnderflowException)
protected:
    const char* typeName() const noexcept override { return "BufferUnderflowException"; }
};

} // namespace jxx::nio