#pragma once

#include "lang/jxx_types.h"
#include "jxx.io.InputStream.h"

namespace jxx::io {

class FilterInputStream : public InputStream {
public:
    explicit FilterInputStream(jxx::Ptr<InputStream> in);

    jxx::lang::jint read() override;
    jxx::lang::jint read(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    jxx::lang::jlong skip(jxx::lang::jlong n) override;
    jxx::lang::jint available() override;
    void close() override;

    void mark(jxx::lang::jint readlimit) override;
    void reset() override;
    jxx::lang::jbool markSupported() override;

protected:
    jxx::Ptr<InputStream> in_;
};

} // namespace jxx::io
