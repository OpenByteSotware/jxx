#include "jxx.lang.StackTrace.h"

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <execinfo.h>
#include <cstring>
#endif

#if defined(__GNUG__)
#include <cxxabi.h>
#include <cstdlib>
#endif

namespace jxx::lang {

    static std::string demangle(const char* name) {
#if defined(__GNUG__)
        int status = 0;
        char* out = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string s = (status == 0 && out) ? out : (name ? name : "");
        std::free(out);
        return s;
#else
        return name ? name : "";
#endif
    }

    std::vector<StackTraceElement> captureStackTrace(std::size_t skipFrames, std::size_t maxFrames) {
        std::vector<StackTraceElement> out;

#if defined(_WIN32)
        // CaptureStackBackTrace returns addresses
        std::vector<void*> frames(maxFrames);
        USHORT captured = ::CaptureStackBackTrace(
            static_cast<DWORD>(skipFrames + 1), // +1 to skip this function
            static_cast<DWORD>(maxFrames),
            frames.data(),
            nullptr
        );

        out.reserve(captured);
        for (USHORT i = 0; i < captured; ++i) {
            StackTraceElement e;
            e.address = reinterpret_cast<std::uintptr_t>(frames[i]);
            // Symbol resolution requires dbghelp; keep best-effort address-only here.
            e.methodName = "<unknown>";
            e.declaringClass = "<unknown>";
            out.push_back(std::move(e));
        }

#elif defined(__unix__) || defined(__APPLE__)
        std::vector<void*> frames(maxFrames);
        int n = ::backtrace(frames.data(), static_cast<int>(maxFrames));
        if (n <= 0) return out;

        char** syms = ::backtrace_symbols(frames.data(), n);
        if (!syms) return out;

        // skipFrames + 1 skips captureStackTrace itself
        std::size_t start = std::min<std::size_t>(skipFrames + 1, static_cast<std::size_t>(n));
        out.reserve(static_cast<std::size_t>(n) - start);

        for (std::size_t i = start; i < static_cast<std::size_t>(n); ++i) {
            StackTraceElement e;
            e.address = reinterpret_cast<std::uintptr_t>(frames[i]);

            // backtrace_symbols gives a human-ish string; parse minimally:
            // Keep it as methodName if we can't parse.
            std::string s = syms[i] ? syms[i] : "";
            e.methodName = s;

            out.push_back(std::move(e));
        }

        std::free(syms);
#else
        // Unsupported platform: return empty
#endif

        return out;
    }

} // namespace jxx::lang