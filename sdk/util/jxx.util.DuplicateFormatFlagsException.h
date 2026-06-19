#pragma once

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class DuplicateFormatFlagsException : public IllegalFormatException {
    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(DuplicateFormatFlagsException)
    protected:
        const char* typeName() const noexcept override { return "DuplicateFormatFlagsException"; }
    };

} // namespace jxx::util
