#pragma once

#include <memory>
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

class NumberFormatException : public IllegalArgumentException {
public:
    using IllegalArgumentException::IllegalArgumentException;
    JXX_THROWABLE_CLONE(NumberFormatException)
protected:
    const char* typeName() const noexcept override { return "NumberFormatException"; }
};

} // namespace jxx::lang
