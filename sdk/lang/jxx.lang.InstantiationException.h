#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class InstantiationException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    JXX_THROWABLE_CLONE(InstantiationException)
protected:
    const char* typeName() const noexcept override { return "InstantiationException"; }
};

} // namespace jxx::lang