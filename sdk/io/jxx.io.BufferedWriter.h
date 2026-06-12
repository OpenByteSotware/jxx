#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.Writer.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::io {

// Java 8 parity: java.io.BufferedWriter
class BufferedWriter final : public Writer {
public:
    explicit BufferedWriter(const jxx::Ptr<Writer> out);
    BufferedWriter(const jxx::Ptr<Writer> out, jxx::lang::jint sz);

    virtual void write(jxx::lang::jint c) override;
    virtual void write(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) override;
    virtual void write(const jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len) override;

    void newLine();

    void flush() override;
    void close() override;

private:
    jxx::Ptr<Writer> out_;
    jxx::Ptr<jxx::lang::CharArrayType> buf_;
    jxx::lang::jint count_ = 0;

    void flushBuffer_();
};

} // namespace jxx::io
