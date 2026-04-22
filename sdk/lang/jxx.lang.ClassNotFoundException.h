#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class ClassNotFoundException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    JXX_THROWABLE_CLONE(ClassNotFoundException)
protected:
    const char* typeName() const noexcept override { return "ClassNotFoundException"; }
};

} // namespace jxx::lang
``