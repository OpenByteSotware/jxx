#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::nio {

class BufferOverflowException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(BufferOverflowException)
protected:
    const char* typeName() const noexcept override { return "BufferOverflowException"; }
};

} // namespace jxx::nio