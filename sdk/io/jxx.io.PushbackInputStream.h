
#pragma once
#include <memory>
#include "jxx/io/jxx.io.FilterInputStream.h"

namespace jxx { namespace io {

class PushbackInputStream : public FilterInputStream {
    ByteArray buf; // pushback buffer
    int pos;       // next byte to read from pushback buffer; equals size when empty
public:
    explicit PushbackInputStream(std::shared_ptr<InputStream> in, int size = 1);

    int read() override;
    jxx::lang::jint read(ByteArray& b, jxx::lang::jint off, jxx::lang::jint len) override;
    void unread(int b);
    void unread(const ByteArray& b, jxx::lang::jint off, jxx::lang::jint len);
    void unread(const ByteArray& b);
    int available() override;
};

}}
