#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class IllegalAccessException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    JXX_THROWABLE_CLONE(IllegalAccessException)
protected:
    const char* typeName() const noexcept override { return "IllegalAccessException"; }
};

} // namespace jxx::lang
``