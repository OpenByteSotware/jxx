
#include <algorithm>
#include <mutex>
#include <atomic>

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#elif defined(__linux__) || defined(__APPLE__)
#include <execinfo.h>
#include <cstdlib>
#include <string>
#endif

#if defined(__GNUG__)
#include <cxxabi.h>
#include <cstdlib>
#endif

#include "jxx.lang.StackTrace.h"

namespace jxx::lang {

    // -------------------- global format option --------------------
    static std::atomic<SymbolFormat> g_symbolFormat{ SymbolFormat::Dotted };
    static std::atomic<std::uint32_t> g_symbolFlags{
        StripReturnTypes | CompressTemplateArgs | PrettifyOperatorCall
    };
     
    void setStackTraceSymbolFormatFlags(std::uint32_t flags) {
        g_symbolFlags.store(flags, std::memory_order_relaxed);
    }
    std::uint32_t getStackTraceSymbolFormatFlags() {
        return g_symbolFlags.load(std::memory_order_relaxed);
    }
    void setStackTraceSymbolFormat(SymbolFormat fmt) {
        g_symbolFormat.store(fmt, std::memory_order_relaxed);
    }

    SymbolFormat getStackTraceSymbolFormat() {
        return g_symbolFormat.load(std::memory_order_relaxed);
    }

    // -------------------- helpers --------------------
    static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
        if (from.empty()) return;
        std::size_t pos = 0;
        while ((pos = s.find(from, pos)) != std::string::npos) {
            s.replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    static std::string toDotted(std::string s) {
        // Convert C++ scope separator to Java dotted format
        replaceAll(s, "::", ".");
        return s;
    }

    static std::string demangleIfNeeded(const std::string& s) {
#if defined(__GNUG__)
        // Itanium ABI mangled names usually begin with "_Z"
        if (s.rfind("_Z", 0) != 0) return s;
        int status = 0;
        char* out = abi::__cxa_demangle(s.c_str(), nullptr, nullptr, &status);
        std::string result = (status == 0 && out) ? out : s;
        std::free(out);
        return result;
#else
        return s;
#endif
    }

    // -------------------- Windows symbolization --------------------
#if defined(_WIN32)

    static std::once_flag g_dbghelpInitOnce;

    static void ensureDbgHelpInitialized() {
        std::call_once(g_dbghelpInitOnce, [] {
            HANDLE proc = GetCurrentProcess();
            // Undecorate + deferred loads; good default for “symbol names only”
            SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
            SymInitialize(proc, nullptr, TRUE);
            });
    }

    static std::string windowsSymbolFromAddress(std::uintptr_t addr) {
        ensureDbgHelpInitialized();

        HANDLE proc = GetCurrentProcess();

        constexpr std::size_t kMaxNameLen = 1024;
        std::uint8_t buffer[sizeof(SYMBOL_INFO) + kMaxNameLen] = {};
        auto* sym = reinterpret_cast<SYMBOL_INFO*>(buffer);

        sym->SizeOfStruct = sizeof(SYMBOL_INFO);
        sym->MaxNameLen = kMaxNameLen;

        DWORD64 displacement = 0;
        if (SymFromAddr(proc, static_cast<DWORD64>(addr), &displacement, sym)) {
            return std::string(sym->Name);
        }
        return std::string("<unknown>");
    }

    static std::string formatWindowsSymbol(std::string sym) {
        auto fmt = getStackTraceSymbolFormat();
        if (fmt == SymbolFormat::Raw) {
            return sym;
        }
        // DbgHelp with SYMOPT_UNDNAME usually returns already “demangled-ish”
        if (fmt == SymbolFormat::Demangled) {
            return sym;
        }
        // Dotted
        return toDotted(sym);
    }

#endif

    // -------------------- Linux symbol parsing --------------------
#if defined(__linux__) || defined(__APPLE__)

// Typical glibc backtrace_symbols format:
//   <module>(<symbol>+<offset>) [<addr>]
    static std::string extractSymbolPortion(const char* symLine) {
        if (!symLine) return "<unknown>";
        std::string line(symLine);

        auto lparen = line.find('(');
        auto rparen = line.find(')', lparen == std::string::npos ? 0 : lparen + 1);

        if (lparen != std::string::npos && rparen != std::string::npos && rparen > lparen + 1) {
            std::string inside = line.substr(lparen + 1, rparen - (lparen + 1));

            auto plus = inside.find('+');
            std::string sym = (plus == std::string::npos) ? inside : inside.substr(0, plus);

            if (!sym.empty()) return sym;
        }

        // Fallback: return whole line if we can’t parse
        return line;
    }

    static std::string formatLinuxSymbol(const char* symLine) {
        auto fmt = getStackTraceSymbolFormat();

        if (fmt == SymbolFormat::Raw) {
            return symLine ? std::string(symLine) : std::string("<unknown>");
        }

        std::string sym = extractSymbolPortion(symLine);
        sym = demangleIfNeeded(sym);

        if (fmt == SymbolFormat::Demangled) {
            return sym; // e.g. ns::Cls::meth()
        }

        // Dotted:
        return toDotted(sym); // e.g. ns.Cls.meth()
    }

#endif

    // -------------------- Public API: captureStackTrace --------------------
    std::vector<StackTraceElement>
        captureStackTrace(std::size_t skipFrames, std::size_t maxFrames) {
        std::vector<StackTraceElement> out;

#if defined(_WIN32)

        std::vector<void*> frames(maxFrames);
        // +1 skips captureStackTrace itself
        USHORT captured = CaptureStackBackTrace(
            static_cast<DWORD>(skipFrames + 1),
            static_cast<DWORD>(maxFrames),
            frames.data(),
            nullptr
        );

        out.reserve(captured);
        for (USHORT i = 0; i < captured; ++i) {
            std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(frames[i]);
            StackTraceElement e;
            e.address = addr;

            std::string sym = windowsSymbolFromAddress(addr);
            e.symbol = formatWindowsSymbol(std::move(sym));

            out.push_back(std::move(e));
        }

#elif defined(__linux__) || defined(__APPLE__)

        std::vector<void*> frames(maxFrames);
        int n = ::backtrace(frames.data(), static_cast<int>(maxFrames));
        if (n <= 0) return out;

        char** syms = ::backtrace_symbols(frames.data(), n);
        if (!syms) return out;

        std::size_t start = std::min<std::size_t>(skipFrames + 1, static_cast<std::size_t>(n));
        out.reserve(static_cast<std::size_t>(n) - start);

        for (std::size_t i = start; i < static_cast<std::size_t>(n); ++i) {
            StackTraceElement e;
            e.address = reinterpret_cast<std::uintptr_t>(frames[i]);
            e.symbol = formatLinuxSymbol(syms[i]);
            out.push_back(std::move(e));
        }

        std::free(syms);

#else
        (void)skipFrames;
        (void)maxFrames;
#endif

        return out;
    }

} // namespace jxx::lang