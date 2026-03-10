
#pragma once
#include <memory>
#include <string>
#include "jxx/io/jxx.io.Writer.h"
#include "jxx/io/jxx.io.OutputStream.h"
#include "jxx/io/jxx.io.OutputStreamWriter.h"

namespace jxx { namespace io {

class PrintWriter : public jxx::lang::Object, public Flushable, public Closeable {
    std::shared_ptr<Writer> out;
    bool autoFlush = false;
    bool error_ = false;
    std::string lineSep = "
";

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
    void print(bool v);
    void print(int v);
    void print(long long v);
    void print(float v);
    void print(double v);

    void println();
    void println(const std::string& s);
    void println(const char* s);
    void println(const std::u16string& s);
    void println(bool v);
    void println(int v);
    void println(long long v);
    void println(float v);
    void println(double v);
};

}}
