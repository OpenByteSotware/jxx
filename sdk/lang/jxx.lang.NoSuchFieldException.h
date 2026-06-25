#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class NoSuchFieldException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    
protected:
    JXX_OBJECT_CLONE(NoSuchFieldException)
    const char* typeName() const noexcept override { return "NoSuchFieldException"; }
};

} // namespace jxx::lang