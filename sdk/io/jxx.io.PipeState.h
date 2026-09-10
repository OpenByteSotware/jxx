#pragma once

#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>

#include "lang/jxx_types.h"

namespace jxx::io::internal {

struct PipeState final {
    explicit PipeState(::jxx::lang::jint capacityValue)
        : buffer(static_cast<std::size_t>(capacityValue)) {
    }

    std::mutex mutex;
    std::condition_variable readable;
    std::condition_variable writable;
    std::vector<::jxx::lang::jbyte> buffer;
    std::size_t readPosition = 0;
    std::size_t writePosition = 0;
    std::size_t count = 0;
    bool connected = false;
    bool inputClosed = false;
    bool outputClosed = false;
};

} // namespace jxx::io::internal
