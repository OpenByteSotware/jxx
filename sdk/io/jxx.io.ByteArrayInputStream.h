#pragma once

#include "lang/jxx_types.h"
#include "jxx.io.InputStream.h"

namespace jxx::io {

    class ByteArrayInputStream final : public InputStream {
    public:
        explicit ByteArrayInputStream(jxx::Ptr<jxx::lang::ByteArray> buf);
        ByteArrayInputStream(jxx::Ptr<jxx::lang::ByteArray> buf, jxx::lang::jint offset, jxx::lang::jint length);

        jxx::lang::jint read() override;
        jxx::lang::jint read(jxx::Ptr<jxx::lang::ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

        jxx::lang::jlong skip(jxx::lang::jlong n) override;
        jxx::lang::jint available() override;

        void mark(jxx::lang::jint readlimit) override;
        void reset() override;
        jxx::lang::jbool markSupported() override;
    private:
        jxx::Ptr<jxx::lang::ByteArray> buf_;
        jxx::lang::jint pos_ = 0;
        jxx::lang::jint mark_ = 0;
        jxx::lang::jint end_ = 0;
    };

} // namespace jxx::io