
#pragma once
#include <memory>
#include <cstring>
#include <algorithm>
#include <limits>

#include "io/jxx.io.InputStream.h"
namespace jxx { namespace io {
    class ByteArrayInputStream : public InputStream {
        std::shared_ptr<const ByteArray> buf; size_t pos=0; size_t markpos=0; size_t count=0;
    public:
        explicit ByteArrayInputStream(const ByteArray& b);
        explicit ByteArrayInputStream(std::shared_ptr<const ByteArray> bsp);
        virtual int read() override;
        virtual int read(ByteArray& b, int off, int len) override;
        virtual int available() override; void mark(int) override { markpos=pos; } 
        virtual void reset() override { pos=markpos; }
        virtual bool markSupported() override { return true; }
    };
}}
