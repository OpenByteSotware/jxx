#pragma once

#include <memory>
#include "jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::lang {

class StringIndexOutOfBoundsException : public IndexOutOfBoundsException {
public:
    using IndexOutOfBoundsException::IndexOutOfBoundsException;
    JXX_THROWABLE_CLONE(StringIndexOutOfBoundsException)
protected:
    const char* typeName() const noexcept override { return "StringIndexOutOfBoundsException"; }
};

} // namespace jxx::lang
