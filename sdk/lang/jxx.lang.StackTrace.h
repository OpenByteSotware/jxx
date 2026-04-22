#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace jxx::lang {

    struct StackTraceElement {
        std::string symbol;          // formatted symbol string
        std::uintptr_t address = 0;  // program counter
    };

    enum class SymbolFormat {
        Raw,        // raw platform string
        Demangled,  // demangled C++ symbol (ns::Class::method())
        Dotted      // Java-ish dotted (ns.Class.method())
    };

    enum SymbolFormatFlags : std::uint32_t {
        None = 0,
        StripReturnTypes = 1u << 0,  // remove "int __cdecl " etc (Windows)
        CompressTemplateArgs = 1u << 1,  // Foo<Bar<Baz>> -> Foo<…>
        PrettifyOperatorCall = 1u << 2,  // operator() -> invoke / call
        // (room for more: StripLambdas, StripStdNoise, etc.)
    };

    // Global configuration
    void setStackTraceSymbolFormat(SymbolFormat fmt);
    SymbolFormat getStackTraceSymbolFormat();

    void setStackTraceSymbolFormatFlags(std::uint32_t flags);
    std::uint32_t getStackTraceSymbolFormatFlags();

    // Capture stack trace (symbol names only)
    std::vector<StackTraceElement>
        captureStackTrace(std::size_t skipFrames = 0, std::size_t maxFrames = 64);

} // namespace jxx::lang
