
#pragma once
#include <memory>
#include <string>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.Writer.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.OutputStreamWriter.h"

namespace jxx { namespace io {

class PrintWriter : public jxx::lang::Object, public Flushable, public Closeable {
    std::shared_ptr<Writer> out;
    jxx::lang::jbool autoFlush = false;
    jxx::lang::jbool error_ = false;
    std::string lineSep = "\n";

    void setError() { error_ = true; }
    void doWrite(const std::u16string& s);
public:
    explicit PrintWriter(std::shared_ptr<Writer> w, bool autoFlush_ = false);
    explicit PrintWriter(std::shared_ptr<OutputStream> os, bool autoFlush_ = false, const std::string& charset = "UTF-8");

    void flush() override;
    void close() override;
    bool checkError() const { return error_; }

    // write/print/println
    void write(const jxx::lang::jchar* buf, int off, int len);
    void write(const std::u16string& s);

    void print(const std::string& s);
    void print(const char* s);
    void print(const std::u16string& s);
    void print(jxx::lang::jbool v);
    void print(jxx::lang::jint v);
    void print(jxx::lang::jlong v);
    void print(jxx::lang::jfloat v);
    void print(jxx::lang::jdouble v);
    void println();
    void println(const std::string& s);
    void println(const char* s);
    void println(const std::u16string& s);
    void println(jxx::lang::jbool v);
    void println(jxx::lang::jint v);
    void println(jxx::lang::jlong v);
    void println(jxx::lang::jfloat v);
    void println(jxx::lang::jdouble v);
};

}}
