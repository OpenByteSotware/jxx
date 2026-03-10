
#pragma once
#include <fstream>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.OutputStream.h"

namespace jxx { namespace io {
class FileOutputStream : public OutputStream { 
std::ofstream ofs; public:
    explicit FileOutputStream(const std::string& path, bool append=false);
    void write(int b) override; 
    void write(const ByteArray& b, int off, int len) override; 
    void flush() override; void close() override; };
}}
