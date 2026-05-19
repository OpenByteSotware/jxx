#pragma once

#include "jxx.io.InputStream.h"
#include "jxx.io.IOException.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.String.h"

#include <condition_variable>
#include <vector>

namespace jxx::io {

class PipedOutputStream;

// Java 8: java.io.PipedInputStream
class PipedInputStream final : public InputStream {
public:
    PipedInputStream();
    explicit PipedInputStream(jxx::Ptr<PipedOutputStream> src);
    explicit PipedInputStream(jxx::lang::jint pipeSize);
    PipedInputStream(jxx::Ptr<PipedOutputStream> src, jxx::lang::jint pipeSize);

    void connect(jxx::Ptr<PipedOutputStream> src);

    jxx::lang::jint read() override;
    jxx::lang::jint read(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len) override;

    jxx::lang::jint available() override;
    void close() override;

    jxx::lang::jbool markSupported() override;

private:
    friend class PipedOutputStream;

    void receive_(jxx::lang::jint b);
    void receive_(jxx::Ptr<ByteArray> b, jxx::lang::jint off, jxx::lang::jint len);
    void receivedLast_();

    void initPipe_(jxx::lang::jint pipeSize);

    std::condition_variable_any notEmpty_;
    std::condition_variable_any notFull_;

    std::vector<jxx::lang::jbyte> buffer_;
    jxx::lang::jint in_ = -1;   // -1 means empty
    jxx::lang::jint out_ = 0;

    jxx::lang::jbool closedByWriter_ = false;
    jxx::lang::jbool closedByReader_ = false;
    jxx::lang::jbool connected_ = false;
};

} // namespace jxx::io
