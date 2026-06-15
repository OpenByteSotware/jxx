#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.FilterReader.h"

namespace jxx::io
{
    class PushbackReader final : public FilterReader
    {
    public:
        explicit PushbackReader(jxx::Ptr<Reader> in);
        PushbackReader(jxx::Ptr<Reader> in, jxx::lang::jint size);
        ~PushbackReader() override = default;

    public:
        jxx::lang::jint read() override;
        jxx::lang::jint read(const jxx::lang::CharArray cbuf,
                             jxx::lang::jint off,
                             jxx::lang::jint len) override;
        void unread(jxx::lang::jint c);
        void unread(const jxx::lang::CharArray cbuf);
        void unread(const jxx::lang::CharArray cbuf,
                    jxx::lang::jint off,
                    jxx::lang::jint len);
        jxx::lang::jbool ready() override;
        jxx::lang::jbool markSupported() const override;
        jxx::lang::jlong skip(jxx::lang::jlong n) override;
        void close() override;

    private:
        jxx::lang::CharArray buf_;
        jxx::lang::jint pos_ = 0;
        jxx::lang::jbool closed_ = false;
    };
}
