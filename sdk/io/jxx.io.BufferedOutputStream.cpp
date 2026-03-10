
#include "io/jxx.io.BufferedOutputStream.h"
#include <cstring>
#include <algorithm>
namespace jxx { namespace io {
BufferedOutputStream::BufferedOutputStream(std::shared_ptr<OutputStream> out_, size_t size): FilterOutputStream(std::move(out_)), buffer(size){}
void BufferedOutputStream::write(int b){ if(count>=buffer.size()) flushBuffer(); buffer[count++]=(uint8_t)(b&0xFF);} 
void BufferedOutputStream::write(const ByteArray& b, int off, int len)
{ 
	if(b.size()< (size_t)off + (size_t)len) throw IOException("IndexOutOfBounds in BufferedOutputStream.write"); if(len >= (int)buffer.size()){ flushBuffer(); 
out->write(b,off,len); return;} if(count + (size_t)len > buffer.size()) flushBuffer(); std::memcpy(buffer.data()+count, b.data()+off, (size_t)len); count += (size_t)len; 
}
void BufferedOutputStream::flush(){ flushBuffer(); out->flush(); }
void BufferedOutputStream::close(){ flush(); out->close(); }
void BufferedOutputStream::flushBuffer(){ if(count>0){ 
	out->write(buffer,0,(int)count); count=0; }
}
}}
