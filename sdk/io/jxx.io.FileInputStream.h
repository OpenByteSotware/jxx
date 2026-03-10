
#pragma once
#include <fstream>
#include <limits>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.InputStream.h"
namespace jxx { namespace io {
class FileInputStream : public InputStream { std::ifstream ifs; public:
    explicit FileInputStream(const std::string& path);
    int read() override; int read(ByteArray& b, int off, int len) override; int available() override; void close() override; };
}}
