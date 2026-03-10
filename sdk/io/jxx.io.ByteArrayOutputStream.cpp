
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.IOException.h"

namespace jxx { namespace io {
ByteArrayOutputStream::ByteArrayOutputStream(size_t size): buf(size), count(0){}
void ByteArrayOutputStream::write(int b){ if(count>=buf.size()) buf.resize(std::max<size_t>(1, buf.size()*2)); buf[count++]=(uint8_t)(b&0xFF);} 
void ByteArrayOutputStream::write(const ByteArray& b, int off, int len){ if(b.size()< (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in ByteArrayOutputStream.write"); ensureCapacity(count + (size_t)len); std::memcpy(buf.data()+count, b.data()+off, (size_t)len); count += (size_t)len; }
void ByteArrayOutputStream::reset(){ count=0; }
ByteArray ByteArrayOutputStream::toByteArray() const { return ByteArray(buf.begin(), buf.begin() + (ptrdiff_t)count); }
void ByteArrayOutputStream::writeTo(OutputStream& out){ out.write(buf,0,(int)count);} 
void ByteArrayOutputStream::ensureCapacity(size_t minCap){ if(minCap>buf.size()){ size_t newCap=std::max(minCap, buf.size()*2); buf.resize(newCap);} }
}}
