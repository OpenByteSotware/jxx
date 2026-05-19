#pragma once

#include "jxx.io.InputStream.h"
#include "jxx.io.IOException.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.String.h"

#include <fstream>

namespace jxx::io {

class FileInputStream final : public InputStream {
public:
    explicit FileInputStream(jxx::Ptr<jxx::lang::String> path);

    jint read() override;
    jint read(jxx::Ptr<ByteArray> b, jint off, jint len) override;

    jint available() override;
    void close() override;

private:
    std::ifstream f_;
};

} // namespace jxx::io
