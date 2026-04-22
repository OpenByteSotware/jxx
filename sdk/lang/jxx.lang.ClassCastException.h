#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ClassCastException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    JXX_THROWABLE_CLONE(ClassCastException)
protected:
    const char* typeName() const noexcept override { return "ClassCastException"; }
};

} // namespace jxx::lang