#pragma once

#include <memory>
#include "jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::lang {

class ArrayIndexOutOfBoundsException : public IndexOutOfBoundsException {
public:
    using IndexOutOfBoundsException::IndexOutOfBoundsException;
    
protected:
    JXX_OBJECT_CLONE(ArrayIndexOutOfBoundsException)
    const char* typeName() const noexcept override { return "ArrayIndexOutOfBoundsException"; }
};

} // namespace jxx::lang