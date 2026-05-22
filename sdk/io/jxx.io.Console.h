
#pragma once
#include <memory>
#include <string>
#include <cstdarg>
#include <vector>

#include "io/jxx.io.Reader.h"
#include "io/jxx.io.InputStreamReader.h"
#include "io/jxx.io.BufferedReader.h"
#include "lang/jxx.lang.Object.h"

namespace jxx { 
    namespace io {
        class Console : public jxx::lang::Object {
        public:
            static jxx::Ptr<Console> console();

            jxx::Ptr<jxx::lang::String> readLine();
            void printf(const char* fmt, ...);

            jxx::Ptr<Reader> reader();
            jxx::Ptr<PrintStream> out();
        private:
            Console() = default;
        };
    }
}
