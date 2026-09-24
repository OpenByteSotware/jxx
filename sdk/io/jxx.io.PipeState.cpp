#include "io/jxx.io.PipeState.h"

namespace jxx::io::internal {

PipeState::PipeState(::jxx::lang::jint capacityValue)
    : buffer(static_cast<std::size_t>(capacityValue)) {
}

} // namespace jxx::io::internal
