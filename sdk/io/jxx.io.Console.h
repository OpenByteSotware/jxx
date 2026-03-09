#pragma once
#include "jxx.lang.Object.h"
#include <string>
#include <cstdarg>
#include <iostream>
#include <vector>
#include <cstdio>
#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

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
            // best-effort reallocation; some platforms require a fresh va_list to recompute
            buf.resize(static_cast<size_t>(n)+1);
        }
        std::cout.write(buf.data(), n);
    }

    std::string readLine(const std::string& prompt = std::string()) {
        if (!prompt.empty()) std::cout << prompt;
        std::string line; std::getline(std::cin, line); return line;
    }

    // Reads a password without echo. Returns the line read. Newline is consumed.
    std::string readPassword(const std::string& prompt = std::string()) {
        if (!prompt.empty()) std::cout << prompt;
#if defined(_WIN32)
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD oldMode = 0;
        GetConsoleMode(hStdin, &oldMode);
        DWORD noEcho = oldMode & ~ENABLE_ECHO_INPUT;
        SetConsoleMode(hStdin, noEcho);
        std::string line; std::getline(std::cin, line);
        SetConsoleMode(hStdin, oldMode);
        std::cout << std::endl;
        return line;
#else
        struct termios oldt; tcgetattr(STDIN_FILENO, &oldt);
        struct termios newt = oldt; newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        std::string line; std::getline(std::cin, line);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::cout << std::endl;
        return line;
#endif
    }
};

}} // namespace jxx::io
