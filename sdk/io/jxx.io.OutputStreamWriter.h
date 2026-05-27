#pragma once

#include "jxx.io.Writer.h"
#include "jxx.io.OutputStream.h"

namespace jxx::lang {
    class String;
}

namespace jxx::io {

// Java 8: java.io.OutputStreamWriter
class OutputStreamWriter final : public Writer {
public:
    explicit OutputStreamWriter(const jxx::Ptr<OutputStream> out);
    OutputStreamWriter(const jxx::Ptr<OutputStream> out, const jxx::Ptr<jxx::lang::String> charsetName);

    void write(jxx::lang::jint c) override;
    void write(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) override;
    void write(const jxx::Ptr<jxx::lang::String> str, jxx::lang::jint off, jxx::lang::jint len) override;

    void flush() override;
    void close() override;

    jxx::Ptr<jxx::lang::String> getEncoding() const;

private:
    enum class Enc { UTF8, UTF16BE, UTF16LE, UTF16_BOM };

    jxx::Ptr<OutputStream> out_;
    Enc enc_ = Enc::UTF8;
    mutable jxx::lang::jbool bomWritten_ = false;

    static Enc parseEncoding_(const jxx::Ptr<jxx::lang::String> name);
    void writeBomIfNeeded_();

    void writeUtf8CodePoint_(jxx::lang::jint cp);
    void writeUtf16CodeUnitBE_(jxx::lang::jint cu);
    void writeUtf16CodeUnitLE_(jxx::lang::jint cu);

    static jxx::lang::jbool isHigh_(jxx::lang::jint cu);
    static jxx::lang::jbool isLow_(jxx::lang::jint cu);
    static jxx::lang::jint toCodePoint_(jxx::lang::jint hi, jxx::lang::jint lo);
};

} // namespace jxx::io
