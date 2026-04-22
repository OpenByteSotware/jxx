#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace jxx::lang {

    struct StackTraceElement {
        std::string declaringClass; // best-effort (often module / namespace-ish)
        std::string methodName;     // best-effort function symbol
        std::string fileName;       // may be empty unless you add debug symbolization
        int lineNumber = -1;        // -1 if unknown
        std::uintptr_t address = 0; // raw program counter
    };

    std::vector<StackTraceElement> captureStackTrace(std::size_t skipFrames = 0,
        std::size_t maxFrames = 64);

} // namespace jxx::lang