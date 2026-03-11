
#pragma once
#include <memory>
#include <string>
#include <cstdarg>
#include <vector>
#include "lang/jxx.lang.System.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.BufferedReader.h"

namespace jxx { namespace io {
class Console {
public:
    static std::shared_ptr<Console> console();

    std::u16string readLine();
    void printf(const char* fmt, ...);

    std::shared_ptr<Reader> reader();
    std::shared_ptr<PrintStream> out();
private:
    Console() = default;
};
}}
