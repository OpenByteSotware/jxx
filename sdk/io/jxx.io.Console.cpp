
#include "io/jxx.io.Console.h"
#include <cstdio>

namespace jxx { namespace io {
static std::shared_ptr<Console> __console_instance;

std::shared_ptr<Console> Console::console(){
    if(!__console_instance) __console_instance = std::shared_ptr<Console>(new Console());
    return __console_instance;
}

std::shared_ptr<Reader> Console::reader(){
    return std::make_shared<InputStreamReader>(jxx::lang::System::in);
}

std::shared_ptr<PrintStream> Console::out(){
    return jxx::lang::System::out;
}

void Console::printf(const char* fmt, ...){
    if(!fmt) return;
    char tmp[1024];
    va_list ap; va_start(ap, fmt);
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    if(n < 0) return;
    if(n < (int)sizeof(tmp)){
        jxx::lang::System::out->print(std::string(tmp, (size_t)n));
        return;
    }
    // allocate dynamically if needed
    std::vector<char> buf((size_t)n + 1);
    va_start(ap, fmt);
    vsnprintf(buf.data(), buf.size(), fmt, ap);
    va_end(ap);
    jxx::lang::System::out->print(std::string(buf.data(), (size_t)n));
}

std::u16string Console::readLine(){
    auto isr = std::make_shared<InputStreamReader>(jxx::lang::System::in);
    BufferedReader br(isr);
    return br.readLine();
}

}} // ns
