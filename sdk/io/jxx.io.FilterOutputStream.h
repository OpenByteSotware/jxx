
#pragma once
#include <memory>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.OutputStream.h"
namespace jxx { namespace io {
class FilterOutputStream : public OutputStream { protected: std::shared_ptr<OutputStream> out; public:
    explicit FilterOutputStream(std::shared_ptr<OutputStream> out_) : out(std::move(out_)) {}
    virtual void write(int b) override; 
    virtual void write(const ByteArray& b, int off, int len) override; 
    void flush() override; void close() override; };
}}
