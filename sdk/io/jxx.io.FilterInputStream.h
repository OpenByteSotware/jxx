#pragma once

#include "jxx.io.InputStream.h"

namespace jxx::io {

class FilterInputStream : public InputStream {
public:
    explicit FilterInputStream(jxx::Ptr<InputStream> in);

    jint read() override;
    jint read(jxx::Ptr<ByteArray> b, jint off, jint len) override;

    jlong skip(jlong n) override;
    jint available() override;
    void close() override;

    void mark(jint readlimit) override;
    void reset() override;
    jbool markSupported() override;

protected:
    jxx::Ptr<InputStream> in_;
};

} // namespace jxx::io
