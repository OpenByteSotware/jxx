#pragma once

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class InstantiationException : public ReflectiveOperationException {
public:
    using ReflectiveOperationException::ReflectiveOperationException;
    
protected:
    JXX_OBJECT_CLONE(InstantiationException)
    const char* typeName() const noexcept override { return "InstantiationException"; }
};

} // namespace jxx::lang