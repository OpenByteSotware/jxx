
#pragma once
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.FilterInputStream.h"
#include "io/jxx.io.DataInput.h"
#include "io/jxx.io.ModifiedUTF.h"
#include "io/jxx.io.EOFException.h"

using namespace jxx::lang;

namespace jxx { namespace io {
class DataInputStream : public FilterInputStream, public DataInput 
{ 
public:
    explicit DataInputStream(std::shared_ptr<InputStream> in) : FilterInputStream(std::move(in)) {}
    void readFully(ByteArray& b) override { readFully(b,0,(int)b.size()); }
    void readFully(ByteArray& b, int off, int len) override { int n=0; while(n<len){ auto r=in->read(b, off+n, len-n); if(r<0) throw EOFException("DataInputStream.readFully: EOF"); n+=r; } }
    int skipBytes(int n) override { return (int)in->skip(n); }
    bool readBoolean() override { int v=in->read(); if(v<0) throw EOFException("readBoolean"); return v!=0; }
    byte readByte() override { int v=in->read(); if(v<0) throw EOFException("readByte"); return (byte)v; }
    char readUnsignedByte() override { int v=in->read(); if(v<0) throw EOFException("readUnsignedByte"); return (char)(v&0xFF); }
    short readShort() override { ByteArray b(2); readFully(b); return (short)((b[0]<<8)|b[1]); }
    unsigned short readUnsignedShort() override { ByteArray b(2); readFully(b); return (unsigned short)((b[0]<<8)|b[1]); }
    char readChar() override { return (char)readUnsignedShort(); }
    int readInt() override { ByteArray b(4); readFully(b); return ((int)b[0]<<24)|((int)b[1]<<16)|((int)b[2]<<8)|((int)b[3]); }
    long readLong() override { ByteArray b(8); readFully(b); long v=0; for(int i=0;i<8;++i) v=(v<<8)|b[i]; return v; }
    float readFloat() override { uint32_t bits=(uint32_t)readInt(); float f; std::memcpy(&f,&bits,4); return f; }
    double readDouble() override { uint64_t bits=(uint64_t)readLong(); double d; std::memcpy(&d,&bits,8); return d; }
    std::u16string readUTF() override {
        unsigned short utflen=readUnsignedShort(); ByteArray bytes(utflen); readFully(bytes);
        return ModifiedUTF::decode(bytes); }
};
}}
