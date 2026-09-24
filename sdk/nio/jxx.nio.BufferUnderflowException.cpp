#include "nio/jxx.nio.BufferUnderflowException.h"

namespace jxx::nio {

const char* BufferUnderflowException::typeName() const noexcept {
    return "BufferUnderflowException";
}

} // namespace jxx::nio
