#include <cstdio>
#include "jxx.io.Console.h"
#include "jxx.io.BufferedReader.h"

using namespace jxx::io;

static std::shared_ptr<jxx::io::Console> __console_instance;

std::shared_ptr<jxx::io::Console> jxx::io::Console::console(){
    if(!__console_instance) __console_instance = JXX_NEW<jxx::io::Console>();
    return __console_instance;
}

std::shared_ptr<Reader> jxx::io::Console::reader(){
    return std::make_shared<InputStreamReader>(jxx::lang::System::in);
}

std::shared_ptr<PrintStream> jxx::io::Console::out(){
    return jxx::lang::System::out;
}

void jxx::io::Console::printf(const char* fmt, ...){
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
    std::vector<char> buf((int)n + 1);
    va_start(ap, fmt);
    vsnprintf(buf.data(), buf.size(), fmt, ap);
    va_end(ap);
    jxx::lang::System::out->print(std::string(buf.data(), (size_t)n));
}

std::u16string jxx::io::Console::readLine(){
    auto isr = std::make_shared<InputStreamReader>(jxx::lang::System::in);
    BufferedReader br(isr);
    return br.readLine();
}


