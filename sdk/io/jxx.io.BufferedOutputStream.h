#pragma once

#include "jxx.io.FilterOutputStream.h"
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::io {

// Java 8: java.io.BufferedOutputStream
class BufferedOutputStream final : public FilterOutputStream {
public:
    explicit BufferedOutputStream(jxx::Ptr<OutputStream> out);
    BufferedOutputStream(jxx::Ptr<OutputStream> out, jxx::lang::jint size);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    void flush() override;
    void close() override;

private:
    jxx::Ptr<ByteArray> buf_;
    jxx::lang::jint count_ = 0;

    void flushBuffer_();
};

} // namespace jxx::io
