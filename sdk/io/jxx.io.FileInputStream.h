#pragma once

#include "jxx.io.InputStream.h"
#include <fstream>

namespace jxx { namespace lang {
    class String;
}
}

namespace jxx::io {

class FileInputStream final : public InputStream {
public:
    FileInputStream() = default;
    explicit FileInputStream(jxx::Ptr<jxx::lang::String> path);

    jxx::lang::jint read() override;
    virtual jxx::lang::jint read(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) override;

    jxx::lang::jint available() override;
    void close() override;

private:
    std::ifstream f_;
   
};

} // namespace jxx::io
