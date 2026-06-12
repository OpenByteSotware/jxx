#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.buildin_array.h"
#include "jxx.io.FilterInputStream.h"

namespace jxx::io {

class DataInputStream final : public FilterInputStream {
public:
    explicit DataInputStream(jxx::Ptr<InputStream> in);

    void readFully(jxx::lang::ByteArray b);
    void readFully(jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len);

    jxx::lang::jint skipBytes(jxx::lang::jint n);

    jxx::lang::jbool readBoolean();
    jxx::lang::jbyte readByte();
    jxx::lang::jint readUnsignedByte();
    jxx::lang::jshort readShort();
    jxx::lang::jint readUnsignedShort();
    jxx::lang::jchar readChar();
    jxx::lang::jint readInt();
    jxx::lang::jlong readLong();
    jxx::lang::jfloat readFloat();
    jxx::lang::jdouble readDouble();
    jxx::Ptr<jxx::lang::String> readLine();
    jxx::Ptr<jxx::lang::String> readUTF();

private:
    void readFullyRaw_(jxx::lang::ByteArray b, jxx::lang::jint off, jxx::lang::jint len);
    jxx::lang::jint readUnsignedShortBE_();
};

} // namespace jxx::io
