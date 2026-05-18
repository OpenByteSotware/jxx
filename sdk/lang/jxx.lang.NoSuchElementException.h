#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class NoSuchElementException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(NoSuchElementException)
protected:
    const char* typeName() const noexcept override { return "NoSuchElementExceptions"; }
};

} // namespace jxx::lang