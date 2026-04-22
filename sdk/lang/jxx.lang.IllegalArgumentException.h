#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class IllegalArgumentException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(IllegalArgumentException)
protected:
    const char* typeName() const noexcept override { return "IllegalArgumentException"; }
};

} // namespace jxx::lang