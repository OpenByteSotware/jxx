#pragma once

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class IllegalFormatCodePointException : public IllegalFormatException {
    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(IllegalFormatCodePointException)
    protected:
        const char* typeName() const noexcept override { return "IllegalFormatCodePointException"; }
    };

} // namespace jxx::util
