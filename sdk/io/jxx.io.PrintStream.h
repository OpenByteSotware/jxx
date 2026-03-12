
#pragma once
#include <memory>
#include <string>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.OutputStreamWriter.h"
#include "lang/jxx.lang.String.h"

using namespace jxx::lang;

namespace jxx { namespace io {
class PrintStream : public jxx::lang::Object, public Flushable, public Closeable { 
    std::shared_ptr<OutputStream> out; std::shared_ptr<OutputStreamWriter> writer; 
    bool autoFlush=false; 
    bool error_=false; 

    // windows and others are different for line seperator
#if defined(_WIN32)
    String lineSep="\n\r"; 
#else
    String lineSep = "\n";
#endif
    void writeString(const String& s);
    void writeU16(const std::u16string& s); 
    void newline(); 
    void handleError();
public:
    explicit PrintStream(std::shared_ptr<OutputStream> out_, bool autoFlush_=false, const std::string& charset="UTF-8"); void flush() override;
    void close() override; bool checkError() const { return error_; }
    void print(const std::string& s); 
    void print(const char* s); 
    void print(const std::u16string& s); 
    void print(bool v); void print(int v); 
    void print(long long v); void print(float v);
    void print(double v);
    void println(); void println(const std::string& s); 
    void println(const char* s); 
    void println(const std::u16string& s); void println(bool v);
    void println(int v); void println(long long v); void println(float v); 
    void println(double v);
};
}}
