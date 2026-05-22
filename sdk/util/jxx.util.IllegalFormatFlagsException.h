#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

class IllegalFormatFlagsException : public jxx::lang::RuntimeException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(IllegalFormatFlagsException)
protected:
    const char* typeName() const noexcept override { return "NoSuchElementException"; }
};

} // namespace jxx::util