#pragma once

#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {

class IndexOutOfBoundsException : public RuntimeException {
public:
    using RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(IndexOutOfBoundsException)

protected:
    const char* typeName() const noexcept override { return "IndexOutOfBoundsException"; }
};

} // namespace jxx::lang