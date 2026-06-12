#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.Reader.h"

namespace jxx::io {

class BufferedReader : public Reader {
public:
    explicit BufferedReader(jxx::Ptr<Reader> in, jxx::lang::jint size = 8192);

    jxx::lang::jint read() override;
    jxx::lang::jint read(const jxx::lang::CharArray cbuf, jxx::lang::jint off, jxx::lang::jint len) override;

    jxx::Ptr<jxx::lang::String> readLine();

    jxx::lang::jbool ready() override;
    void close() override;

private:
    jxx::Ptr<Reader> in_;
    jxx::lang::CharArray buf_;
    jxx::lang::jint pos_ = 0;
    jxx::lang::jint count_ = 0;

    void fill_();
};

} // namespace jxx::io
