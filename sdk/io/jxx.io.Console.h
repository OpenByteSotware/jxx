
#pragma once
#include <memory>
#include <string>
#include <cstdarg>
#include <vector>
#include "lang/jxx.lang.System.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.BufferedReader.h"

namespace jxx { 
    namespace io {
        class Console {
        public:
            static JXX_PTR(Console) console();

            std::u16string readLine();
            void printf(const char* fmt, ...);

            JXX_PTR(Reader) reader();
            JXX_PTR(PrintStream) out();
        private:
            Console() = default;
        };
    }
}
