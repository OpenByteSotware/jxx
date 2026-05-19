#pragma once

#include "jxx.io.OutputStream.h"
#include "jxx.io.IOException.h"

#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.String.h"

namespace jxx::io {

class PipedInputStream;

// Java 8: java.io.PipedOutputStream
class PipedOutputStream final : public OutputStream {
public:
    PipedOutputStream();
    explicit PipedOutputStream(jxx::Ptr<PipedInputStream> snk);

    void connect(jxx::Ptr<PipedInputStream> snk);

    void write(jxx::lang::jint b) override;
    void write(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    void flush() override;
    void close() override;

private:
    jxx::Ptr<PipedInputStream> sink_;
    jxx::lang::jbool connected_ = false;
    jxx::lang::jbool closed_ = false;
};

} // namespace jxx::io
