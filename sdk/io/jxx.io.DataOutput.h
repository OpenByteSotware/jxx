
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"

namespace jxx { namespace io {
class DataOutput {
public:
    virtual ~DataOutput() = default;
    virtual void write(jint b) = 0;
    virtual void write(const ByteArray& b) = 0;
    virtual void write(const ByteArray& b, int off, int len) = 0;
    virtual void writeBoolean(bool v) = 0;
    virtual void writeByte(jbyte v) = 0;
    virtual void writeShort(jshort v) = 0;
    virtual void writeChar(jchar v) = 0;
    virtual void writeInt(jint v) = 0;
    virtual void writeLong(jlong v) = 0;
    virtual void writeFloat(jfloat v) = 0;
    virtual void writeDouble(jdouble v) = 0;
    virtual void writeUTF(const std::u16string& s) = 0;
};
}}
