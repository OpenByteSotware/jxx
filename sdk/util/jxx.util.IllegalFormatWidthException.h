#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

class IllegalFormatWidthException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(IllegalFormatWidthException)
protected:
    const char* typeName() const noexcept override { return "IllegalFormatWidthException"; }
};

} // namespace jxx::util