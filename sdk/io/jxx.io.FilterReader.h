#pragma once

#include "lang/jxx_types.h"
#include "jxx.io.Reader.h"

namespace jxx::io
{
    class FilterReader : public Reader
    {
    protected:
        jxx::Ptr<Reader> in_;

    public:
        explicit FilterReader(jxx::Ptr<Reader> in);
        ~FilterReader() override = default;

    public:
        jxx::lang::jint read() override;
        jxx::lang::jint read(const jxx::lang::CharArray cbuf,
                             jxx::lang::jint off,
                             jxx::lang::jint len) override;
        jxx::lang::jlong skip(jxx::lang::jlong n) override;
        jxx::lang::jbool ready() override;
        jxx::lang::jbool markSupported() const override;
        void mark(jxx::lang::jint readAheadLimit) override;
        void reset() override;
        void close() override;
    };
}
