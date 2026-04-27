#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ClassCastException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(jxx::lang::ClassCastException)
protected:
    const char* typeName() const noexcept override { return "ClassCastException"; }
};

} // namespace jxx::lang