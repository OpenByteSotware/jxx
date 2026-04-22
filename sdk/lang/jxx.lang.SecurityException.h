#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class SecurityException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(SecurityException)
protected:
    const char* typeName() const noexcept override { return "SecurityException"; }
};

} // namespace jxx::lang