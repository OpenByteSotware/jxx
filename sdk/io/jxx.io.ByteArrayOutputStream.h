#pragma once

#include "jxx.io.OutputStream.h"
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::io {

// Java 8: java.io.ByteArrayOutputStream
class ByteArrayOutputStream final : public OutputStream {
public:
    ByteArrayOutputStream();
    explicit ByteArrayOutputStream(jxx::lang::jint size);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    void reset();
    jxx::lang::jint size() const;
    jxx::Ptr<ByteArray> toByteArray() const;
    void writeTo(jxx::Ptr<OutputStream> out) const;

    void close() override;

private:
    jxx::Ptr<ByteArray> buf_;
    jxx::lang::jint count_ = 0;

    void ensureCapacity_(jxx::lang::jint minCapacity);
};

} // namespace jxx::io
