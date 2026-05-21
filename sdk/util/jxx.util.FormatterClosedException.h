#pragma once

#include <memory>
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx::util {

class FormatterClosedException : public jxx::lang::IllegalStateException {
public:
    using jxx::lang::IllegalStateException::IllegalStateException;
    JXX_OBJECT_CLONE(FormatterClosedException)
protected:
    const char* typeName() const noexcept override { return "FormatterClosedException"; }
};

} // namespace jxx::util