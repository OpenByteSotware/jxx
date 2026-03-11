
#pragma once
#include <fstream>
#include <ostream>
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.FileDescriptor.h"
namespace jxx { namespace io {
class FileOutputStream : public OutputStream { 
    std::ofstream ofs;            // owned file stream when opened by path
    std::ostream* ext = nullptr;  // non-owning pointer to external stream (e.g., std::cout/cerr)
public:
    explicit FileOutputStream(const std::string& path, bool append=false);
    explicit FileOutputStream(const FileDescriptor& fd);
    void write(int b) override;
    void write(const ByteArray& b, jxx::lang::jint off, jxx::lang::jint len) override;
    void flush() override;
    void close() override;
};
}}
