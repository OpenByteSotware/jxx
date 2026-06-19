#pragma once

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class IllegalFormatConversionException : public IllegalFormatException {
    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(IllegalFormatConversionException)
    protected:
        const char* typeName() const noexcept override { return "IllegalFormatConversionException"; }
    };

} // namespace jxx::util
