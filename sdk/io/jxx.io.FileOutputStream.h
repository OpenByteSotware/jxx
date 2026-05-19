#pragma once

#include "jxx.io.OutputStream.h"
#include "jxx.io.IOException.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.String.h"

#include <fstream>

namespace jxx::io {

class FileOutputStream final : public OutputStream {
public:
    explicit FileOutputStream(jxx::Ptr<jxx::lang::String> path);
    FileOutputStream(jxx::Ptr<jxx::lang::String> path, jxx::lang::jbool append);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    void flush() override;
    void close() override;

private:
    std::ofstream f_;
};

} // namespace jxx::io
