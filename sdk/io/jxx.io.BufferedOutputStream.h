#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.FilterOutputStream.h"

namespace jxx::io {

class BufferedOutputStream final : public FilterOutputStream {
public:
    explicit BufferedOutputStream(jxx::Ptr<OutputStream> out);
    BufferedOutputStream(jxx::Ptr<OutputStream> out, jxx::lang::jint size);

    virtual void write(jxx::lang::jint b) override;
    virtual void write(const jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len) override;

    virtual void flush() override;
    virtual void close() override;

private:
    jxx::lang::ByteArray buf_;
    jxx::lang::jint count_ = 0;
    void flushBuffer_();
};

} // namespace jxx::io
