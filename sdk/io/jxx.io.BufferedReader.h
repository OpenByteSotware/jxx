#pragma once

#include "jxx.io.Reader.h"
#include "jxx.lang.String.h"

namespace jxx::io {

class BufferedReader : public Reader {
public:
    explicit BufferedReader(jxx::Ptr<Reader> in, jint size = 8192);

    jint read() override;
    jint read(jxx::Ptr<CharArray> cbuf, jint off, jint len) override;

    jxx::Ptr<jxx::lang::String> readLine();

    jbool ready() override;
    void close() override;

private:
    jxx::Ptr<Reader> in_;
    jxx::Ptr<CharArray> buf_;
    jint pos_ = 0;
    jint count_ = 0;

    void fill_();
};

} // namespace jxx::io
