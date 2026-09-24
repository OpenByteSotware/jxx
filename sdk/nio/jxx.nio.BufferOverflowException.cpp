#include "nio/jxx.nio.BufferOverflowException.h"

namespace jxx::nio {

const char* BufferOverflowException::typeName() const noexcept {
    return "BufferOverflowException";
}

} // namespace jxx::nio
