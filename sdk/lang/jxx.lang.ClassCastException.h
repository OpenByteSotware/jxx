#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ClassCastException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    
protected:
    JXX_OBJECT_CLONE(ClassCastException)
    const char* typeName() const noexcept override { return "ClassCastException"; }
};

} // namespace jxx::lang