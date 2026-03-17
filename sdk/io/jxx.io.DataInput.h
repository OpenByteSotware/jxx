
#pragma once
#include <string>
#include "lang/jxx.lang.internal.h"

using namespace jxx::lang;

namespace jxx { namespace io {
class DataInput {
public:
    virtual ~DataInput() = default;
    virtual void readFully(ByteArray& b) = 0;
    virtual void readFully(ByteArray& b, jint off, jint len) = 0;
    virtual jint skipBytes(jint n) = 0;
    virtual jbool readBoolean() = 0;
    virtual jbyte readByte() = 0; 
    virtual jshort readShort() = 0;
    virtual jchar readChar() = 0;
    virtual jint readInt() = 0;
    virtual jlong readLong() = 0;
    virtual jfloat readFloat() = 0;
    virtual jdouble readDouble() = 0;
    virtual std::u16string readUTF() = 0;
};
}}
