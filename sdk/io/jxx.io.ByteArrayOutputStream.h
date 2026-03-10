
#pragma once
#include <cstddef>
#include <algorithm>
#include <cstring>
#include "io/jxx.io.OutputStream.h"

namespace jxx { namespace io {
class ByteArrayOutputStream : public OutputStream { ByteArray buf; size_t count=0; public:
    explicit ByteArrayOutputStream(size_t size=32);
    void write(int b) override;
    void write(const ByteArray& b, int off, int len) override; 
    void reset(); size_t size() const { return count; } 
    ByteArray toByteArray() const; 
    void writeTo(OutputStream& out);
private: void ensureCapacity(size_t minCap);
};
}}
