#pragma once

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class FormatFlagsConversionMismatchException : public IllegalFormatException {
    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(FormatFlagsConversionMismatchException)
    protected:
        const char* typeName() const noexcept override { return "FormatFlagsConversionMismatchException"; }
    };

} // namespace jxx::util
