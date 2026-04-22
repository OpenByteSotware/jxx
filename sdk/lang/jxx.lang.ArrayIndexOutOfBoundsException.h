#pragma once

#include <memory>
#include "jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::lang {

class ArrayIndexOutOfBoundsException : public IndexOutOfBoundsException {
public:
    using IndexOutOfBoundsException::IndexOutOfBoundsException;
    JXX_THROWABLE_CLONE(ArrayIndexOutOfBoundsException)
protected:
    const char* typeName() const noexcept override { return "ArrayIndexOutOfBoundsException"; }
};

} // namespace jxx::lang