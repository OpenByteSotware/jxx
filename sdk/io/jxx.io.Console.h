#pragma once
#include "jxx.lang.Object.h"
#include <string>
#include <cstdarg>
#include <iostream>
#include <vector>

namespace jxx { namespace io {

class Console : public jxx::lang::Object {
public:
    Console() = default;

    void printf(const char* fmt, ...) {
        va_list ap; va_start(ap, fmt); vprintf(fmt, ap); va_end(ap);
    }

    void vprintf(const char* fmt, va_list ap) {
        std::vector<char> buf(1024);
        int n = vsnprintf(buf.data(), buf.size(), fmt, ap);
        if (n < 0) return;
        if (static_cast<size_t>(n) >= buf.size()) {
            buf.resize(n+1);
            // Note: a second vsnprintf would require a fresh va_list copy on some platforms.
        }
        std::cout.write(buf.data(), n);
    }

    std::string readLine(const std::string& prompt = std::string()) {
        if (!prompt.empty()) std::cout << prompt;
        std::string line; std::getline(std::cin, line); return line;
    }
};

}} // namespace jxx::io
