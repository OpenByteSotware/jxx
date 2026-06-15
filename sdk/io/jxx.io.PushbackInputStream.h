#pragma once

#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "io/jxx.io.FilterInputStream.h"

namespace jxx::io
{
    class PushbackInputStream final : public FilterInputStream
    {
    public:
        explicit PushbackInputStream(jxx::Ptr<InputStream> in);
        PushbackInputStream(jxx::Ptr<InputStream> in, jxx::lang::jint size);

        ~PushbackInputStream() override = default;

    public:
        jxx::lang::jint read() override;
        jxx::lang::jint read(const jxx::lang::ByteArray b,
                             jxx::lang::jint off,
                             jxx::lang::jint len) override;

        void unread(jxx::lang::jint b);
        void unread(const jxx::lang::ByteArray b);
        void unread(const jxx::lang::ByteArray b,
                    jxx::lang::jint off,
                    jxx::lang::jint len);

        jxx::lang::jint available() override;
        jxx::lang::jlong skip(jxx::lang::jlong n) override;
        jxx::lang::jbool markSupported() const override;

    private:
        jxx::lang::ByteArray buf_;
        jxx::lang::jint pos_ = 0;
    };
}
