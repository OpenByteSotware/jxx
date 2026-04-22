#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::util {

class ConcurrentModificationException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(ConcurrentModificationException)
protected:
    const char* typeName() const noexcept override { return "ConcurrentModificationException"; }
};

} // namespace jxx::util