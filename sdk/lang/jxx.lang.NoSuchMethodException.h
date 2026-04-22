#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class NoSuchMethodException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    JXX_THROWABLE_CLONE(NoSuchMethodException)
protected:
    const char* typeName() const noexcept override { return "NoSuchMethodException"; }
};

} // namespace jxx::lang
``