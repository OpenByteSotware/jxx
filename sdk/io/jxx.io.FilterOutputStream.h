#pragma once

#include "jxx.io.OutputStream.h"

namespace jxx::io {

// Java 8: java.io.FilterOutputStream
class FilterOutputStream : public OutputStream {
public:
    explicit FilterOutputStream(jxx::Ptr<OutputStream> out);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;
    void flush() override;
    void close() override;

protected:
    jxx::Ptr<OutputStream> out_;
};

} // namespace jxx::io
