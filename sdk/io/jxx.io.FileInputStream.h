
#pragma once
#include <fstream>
#include <istream>
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.FileDescriptor.h"
namespace jxx { namespace io {
class FileInputStream : public InputStream {
    std::ifstream ifs;           // owned file stream when opened by path
    std::istream* ext = nullptr; // non-owning pointer to external stream (e.g., std::cin)
public:
    explicit FileInputStream(const std::string& path);
    explicit FileInputStream(const FileDescriptor& fd);
    int read() override;
    jxx::lang::jint read(ByteArray& b, jxx::lang::jint off, jxx::lang::jint len) override;
    int available() override;
    void close() override;
};
}}
