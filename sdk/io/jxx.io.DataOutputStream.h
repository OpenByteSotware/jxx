#pragma once

#include "jxx.io.FilterOutputStream.h"
#include "jxx.io.IOException.h"
#include "jxx.lang.String.h"

namespace jxx::io {

// Java 8: java.io.DataOutputStream
class DataOutputStream final : public FilterOutputStream {
public:
    explicit DataOutputStream(jxx::Ptr<OutputStream> out);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    void writeBoolean(jxx::lang::jbool v);
    void writeByte(jxx::lang::jint v);
    void writeShort(jxx::lang::jint v);
    void writeChar(jxx::lang::jint v);
    void writeInt(jxx::lang::jint v);
    void writeLong(jxx::lang::jlong v);
    void writeFloat(jxx::lang::jfloat v);
    void writeDouble(jxx::lang::jdouble v);

    void writeBytes(jxx::Ptr<jxx::lang::String> s);   // low 8 bits of each char
    void writeChars(jxx::Ptr<jxx::lang::String> s);   // UTF-16BE
    void writeUTF(jxx::Ptr<jxx::lang::String> s);     // modified UTF-8

    jxx::lang::jint size() const;

private:
    jxx::lang::jint written_ = 0;
    void inc_(jxx::lang::jint n) { written_ += n; }
};

} // namespace jxx::io
