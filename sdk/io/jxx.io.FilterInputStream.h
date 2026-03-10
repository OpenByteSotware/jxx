
#pragma once
#include <memory>
#include "io/jxx.io.InputStream.h"

namespace jxx { namespace io {

class FilterInputStream : public InputStream 
{ 
protected: 
    std::shared_ptr<InputStream> in; 
public:

    explicit FilterInputStream(std::shared_ptr<InputStream> in_) : in(std::move(in_)) {}
    int read() override; 
    int read(ByteArray& b, int off, int len) override;
    int skip(int n) override; 
    int available() override;
    void close() override;
    void mark(int readlimit) override; 
    void reset() override;
    bool markSupported() override; 
};
}}
