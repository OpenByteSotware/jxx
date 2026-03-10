
#pragma once
#include <cstddef>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.FilterInputStream.h"

using namespace jxx:: lang;

namespace jxx { namespace io {
class BufferedInputStream : public FilterInputStream 
{ 
    ByteArray buffer; size_t count=0; size_t pos=0; size_t markpos=(size_t)-1; int marklimit=0;
public:
    explicit BufferedInputStream(std::shared_ptr<InputStream> in, size_t size=8192);
    int read() override; 
    int read(ByteArray& b, int off, int len) override;
    int available() override;
    void mark(int readlimit_) override;
    void reset() override; 
    bool markSupported() override { return true; }
private: int fill(); };
}}
