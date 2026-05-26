#pragma once

#include "jxx.io.OutputStream.h"
#include "jxx.lang.IllegalArgumentException.h"

namespace jxx::io {

class ByteArrayOutputStream final : public OutputStream {
public:
    ByteArrayOutputStream();
    explicit ByteArrayOutputStream(jxx::lang::jint size);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<jxx::lang::ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    void reset();
    jxx::lang::jint size() const;
    jxx::Ptr<jxx::lang::ByteArray> toByteArray() const;
    void writeTo(jxx::Ptr<OutputStream> out) const;

    void close() override;

private:
    jxx::Ptr<jxx::lang::ByteArray> buf_;
    jxx::lang::jint count_ = 0;
    void ensureCapacity_(jxx::lang::jint minCapacity);
};

} // namespace jxx::io
