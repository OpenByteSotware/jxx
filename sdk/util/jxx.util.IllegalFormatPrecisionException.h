#pragma once

#include <memory>
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util {

class IllegalFormatPrecisionException : public jxx::util::IllegalFormatException {
public:
    using jxx::util::IllegalFormatException::IllegalFormatException;
    JXX_OBJECT_CLONE(IllegalFormatPrecisionException)
protected:
    const char* typeName() const noexcept override { return "IllegalFormatPrecisionException"; }
};

} // namespace jxx::util