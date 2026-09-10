#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.IOHelper.h"
namespace jxx::io {
void OutputStream::write(const ::jxx::lang::ByteArray& buffer){IOHelper::checkBounds(buffer,0,static_cast<::jxx::lang::jint>(buffer->length));write(buffer,0,static_cast<::jxx::lang::jint>(buffer->length));}
void OutputStream::write(const ::jxx::lang::ByteArray& buffer,::jxx::lang::jint offset,::jxx::lang::jint length){IOHelper::checkBounds(buffer,offset,length);for(::jxx::lang::jint i=0;i<length;++i)write(static_cast<::jxx::lang::jint>((*buffer)[offset+i])&0xff);}
void OutputStream::flush(){} void OutputStream::close(){}
} // namespace jxx::io
