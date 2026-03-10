
#pragma once
#include <fstream>
#include "lang/jxx.lang.internal.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.DataInput.h"
#include "io/jxx.io.DataOutput.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.ModifiedUTF.h"

namespace jxx { namespace io {
class RandomAccessFile : public jxx::lang::Object, public Closeable, public DataInput, public DataOutput
{
std::fstream fs; public:
    RandomAccessFile(const std::string& path, const std::string& mode);
    void close() override; jxx::lang::jlong getFilePointer(); 
    void seek(jxx::lang::jlong pos); jxx::lang::jlong length(); 
    void setLength(jxx::lang::jlong newLen);
    void write(int b) override; void write(const ByteArray& b) override; 
    void write(const ByteArray& b, int off, int len) override; 
    void writeBoolean(bool v) override; void writeByte(int v) override; 
    void writeShort(int v) override; void writeChar(int v) override; 
    void writeInt(int v) override; void writeLong(jxx::lang::jlong v) override; 
    void writeFloat(float v) override; void writeDouble(double v) override; 
    void writeUTF(const std::u16string& s) override; void readFully(ByteArray& b) override; 
    void readFully(ByteArray& b, int off, int len) override; int skipBytes(int n) override; 
    bool readBoolean() override; jxx::lang::jbyte readByte() override; char readUnsignedByte() override; 
    jxx::lang::jshort readShort() override; unsigned short readUnsignedShort() override;
    jxx::lang::jchar readChar() override; int readInt() override; 
    jxx::lang::jlong readLong() override; 
    float readFloat() override; double readDouble() override; 
    std::u16string readUTF() override; };
}}
