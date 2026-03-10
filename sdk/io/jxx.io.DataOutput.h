
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"

namespace jxx { namespace io {
class DataOutput {
public:
    virtual ~DataOutput() = default;
    virtual void write(int b) = 0;
    virtual void write(const ByteArray& b) = 0;
    virtual void write(const ByteArray& b, int off, int len) = 0;
    virtual void writeBoolean(bool v) = 0;
    virtual void writeByte(int v) = 0;
    virtual void writeShort(int v) = 0;
    virtual void writeChar(int v) = 0;
    virtual void writeInt(int v) = 0;
    virtual void writeLong(long v) = 0;
    virtual void writeFloat(float v) = 0;
    virtual void writeDouble(double v) = 0;
    virtual void writeUTF(const std::u16string& s) = 0;
};
}}
