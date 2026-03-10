
#pragma once
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.FilterOutputStream.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.ModifiedUTF.h"

namespace jxx { namespace io {
class DataOutputStream : public FilterOutputStream, public DataOutput
{ 
    long written_=0; 
public:
    explicit DataOutputStream(std::shared_ptr<OutputStream> out) : FilterOutputStream(std::move(out)) {}
    void write(int b) override { out->write(b); written_+=1; }
    void write(const ByteArray& b) override { write(b,0,(int)b.size()); }
    void write(const ByteArray& b, int off, int len) override { out->write(b,off,len); written_+=len; }
    void writeBoolean(bool v) override { write(v?1:0); }
    void writeByte(int v) override { write(v); }
    void writeShort(int v) override { ByteArray t{ (uint8_t)((v>>8)&0xFF), (uint8_t)(v&0xFF)}; write(t); }
    void writeChar(int v) override { writeShort(v); }
    void writeInt(int v) override { ByteArray t(4); t[0]=(uint8_t)((v>>24)&0xFF); t[1]=(uint8_t)((v>>16)&0xFF); t[2]=(uint8_t)((v>>8)&0xFF); t[3]=(uint8_t)(v&0xFF); write(t); }
    void writeLong(long v) override { ByteArray t(8); for(int i=7;i>=0;--i){ t[7-i]=(uint8_t)((v>>(i*8))&0xFF);} write(t); }
    void writeFloat(float v) override { uint32_t bits; std::memcpy(&bits,&v,4); writeInt((int)bits); }
    void writeDouble(double v) override { uint64_t bits; std::memcpy(&bits,&v,8); writeLong((long)bits); }
    void writeUTF(const std::u16string& s) override { 
        ByteArray enc=ModifiedUTF::encode(s); 
        if(enc.size()>65535) throw UTFDataFormatException("writeUTF: length exceeds 65535"); 
        writeShort((int)enc.size()); write(enc); }
    long size() const { return written_; }
};
}}
