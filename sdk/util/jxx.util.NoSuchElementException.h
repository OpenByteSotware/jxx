#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::util {

class NoSuchElementException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(NoSuchElementException)
protected:
    const char* typeName() const noexcept override { return "NoSuchElementException"; }
};

} // namespace jxx::util