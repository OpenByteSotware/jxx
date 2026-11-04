#pragma once

#include <atomic>
#include <memory>

namespace jxx::lang::ref::detail {

struct ReferenceState final {
    std::atomic<bool> cleared{false};
    std::atomic<bool> enqueued{false};
    std::atomic<bool> dequeued{false};
};

} // namespace jxx::lang::ref::detail
