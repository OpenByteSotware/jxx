#pragma once

#include "jxx.io.FilterOutputStream.h"
#include "jxx.lang.String.h"

namespace jxx::io {

// Java 8: java.io.PrintStream (minimal)
class PrintStream final : public FilterOutputStream {
public:
    explicit PrintStream(jxx::Ptr<OutputStream> out, jxx::lang::jbool autoFlush = false);

    void flush() override;
    void close() override;

    jxx::lang::jbool checkError() const;

    void print(jxx::Ptr<jxx::lang::String> s);
    void print(jxx::lang::jbool b);
    void print(jxx::lang::jint i);
    void print(jxx::lang::jlong l);
    void print(jxx::lang::jfloat f);
    void print(jxx::lang::jdouble d);
    void print(jxx::lang::jchar c);

    void println();
    void println(jxx::Ptr<jxx::lang::String> s);

private:
    jxx::lang::jbool autoFlush_;
    mutable jxx::lang::jbool trouble_ = false;

    void writeBytes_(const std::string& bytes);
    void setTrouble_() const { trouble_ = true; }
};

} // namespace jxx::io
