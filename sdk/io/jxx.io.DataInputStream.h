#pragma once

#include "jxx.io.FilterInputStream.h"
#include "jxx.io.EOFException.h"
#include "jxx.io.IOException.h"
#include "jxx.lang.String.h"

namespace jxx::io {

class DataInputStream final : public FilterInputStream {
public:
    explicit DataInputStream(jxx::Ptr<InputStream> in);

    void readFully(jxx::Ptr<ByteArray> b);
    void readFully(jxx::Ptr<ByteArray> b, jint off, jint len);

    jint skipBytes(jint n);

    jbool readBoolean();
    jbyte readByte();
    jint readUnsignedByte();
    jshort readShort();
    jint readUnsignedShort();
    jchar readChar();
    jint readInt();
    jlong readLong();
    jfloat readFloat();
    jdouble readDouble();

    jxx::Ptr<jxx::lang::String> readLine();
    jxx::Ptr<jxx::lang::String> readUTF();

private:
    void readFullyRaw_(jxx::Ptr<ByteArray> b, jint off, jint len);
    jint readUnsignedShortBE_();
};

} // namespace jxx::io
