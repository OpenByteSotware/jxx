#pragma once

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class UnknownFormatFlagsException : public IllegalFormatException {
    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(UnknownFormatFlagsException)
    protected:
        const char* typeName() const noexcept override { return "UnknownFormatFlagsException"; }
    };

} // namespace jxx::util
