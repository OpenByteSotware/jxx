#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::nio {

class InvalidMarkException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(InvalidMarkException)
protected:
    const char* typeName() const noexcept override { return "InvalidMarkException"; }
};

} // namespace jxx::nio