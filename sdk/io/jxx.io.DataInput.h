
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"

using namespace jxx::lang;

namespace jxx { namespace io {
class DataInput {
public:
    virtual ~DataInput() = default;
    virtual void readFully(ByteArray& b) = 0;
    virtual void readFully(ByteArray& b, int off, int len) = 0;
    virtual int skipBytes(int n) = 0;
    virtual bool readBoolean() = 0;
    virtual byte readByte() = 0;
    virtual char readUnsignedByte() = 0;
    virtual short readShort() = 0;
    virtual unsigned short readUnsignedShort() = 0;
    virtual char readChar() = 0;
    virtual int readInt() = 0;
    virtual long readLong() = 0;
    virtual float readFloat() = 0;
    virtual double readDouble() = 0;
    virtual std::u16string readUTF() = 0;
};
}}
