#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::util {

class FormatterClosedException : public jxx::lang::IllegalStateException {
public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(FormatterClosedException)
protected:
    const char* typeName() const noexcept override { return "FormatterClosedException"; }
};

} // namespace jxx::util