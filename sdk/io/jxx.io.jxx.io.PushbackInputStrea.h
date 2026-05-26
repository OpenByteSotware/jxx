
#pragma once
#include <memory>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.FilterInputStream.h"

namespace jxx { namespace io {

class PushbackInputStream : public FilterInputStream {
    jxx::lang::ByteArray buf; // pushback buffer
    int pos;       // next byte to read from pushback buffer; equals size when empty
public:
    explicit PushbackInputStream(std::shared_ptr<InputStream> in, int size = 1);

    int read() override;
    int read(jxx::lang::ByteArray& b, int off, int len) override;
    void unread(int b);
    void unread(const jxx::lang::ByteArray& b, int off, int len);
    void unread(const jxx::lang::ByteArray& b);
    int available() override;
};

}}
