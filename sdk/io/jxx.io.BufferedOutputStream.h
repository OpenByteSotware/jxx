
#pragma once
#include <cstddef>
#include "io/jxx.io.FilterOutputStream.h"

namespace jxx { namespace io {
class BufferedOutputStream : public FilterOutputStream
{ ByteArray buffer; size_t count=0; 
public:
    explicit BufferedOutputStream(std::shared_ptr<OutputStream> out, size_t size=8192);
    virtual void write(int b) override; 
    virtual void write(const ByteArray& b, int off, int len) override; 
    void flush() override; 
    void close() override;
private: void flushBuffer(); };
}}
