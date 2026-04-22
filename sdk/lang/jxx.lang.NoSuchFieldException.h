#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class NoSuchFieldException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    JXX_THROWABLE_CLONE(NoSuchFieldException)
protected:
    const char* typeName() const noexcept override { return "NoSuchFieldException"; }
};

} // namespace jxx::lang