#include "util/zip/jxx.util.zip.InflaterInputStream.h"

#include <algorithm>

#include "io/jxx.io.EOFException.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/zip/jxx.util.zip.DataFormatException.h"
#include "util/zip/jxx.util.zip.ZipException.h"

namespace jxx::util::zip {
namespace { constexpr ::jxx::lang::jint DEFAULT_BUFFER_SIZE=512; }
InflaterInputStream::InflaterInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input)
 : InflaterInputStream(input,::jxx::NEW<Inflater>(),DEFAULT_BUFFER_SIZE){usesDefaultInflater_=true;}
InflaterInputStream::InflaterInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input,const ::jxx::Ptr<Inflater>& inflater)
 : InflaterInputStream(input,inflater,DEFAULT_BUFFER_SIZE){}
InflaterInputStream::InflaterInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input,const ::jxx::Ptr<Inflater>& inflater,::jxx::lang::jint size)
 : Super(input),inf(inflater){if(inf==nullptr)throw ::jxx::lang::NullPointerException();if(size<=0)throw ::jxx::lang::IllegalArgumentException();buf=::jxx::NEW<::jxx::lang::ByteArrayType>(size);}
InflaterInputStream::~InflaterInputStream(){if(usesDefaultInflater_&&inf!=nullptr)inf->end();}
void InflaterInputStream::ensureOpen_()const{if(closed_)throw ::jxx::io::IOException("stream closed");}
::jxx::lang::jint InflaterInputStream::read(){auto one=::jxx::NEW<::jxx::lang::ByteArrayType>(1);const auto count=read(one,0,1);return count==-1?-1:static_cast<::jxx::lang::jint>(static_cast<unsigned char>((*one)[0]));}
::jxx::lang::jint InflaterInputStream::read(const ::jxx::lang::ByteArray& b){if(b==nullptr)throw ::jxx::lang::NullPointerException();return read(b,0,b->length);}
::jxx::lang::jint InflaterInputStream::read(const ::jxx::lang::ByteArray& b,::jxx::lang::jint off,::jxx::lang::jint length){
 ensureOpen_();if(b==nullptr)throw ::jxx::lang::NullPointerException();if(off<0||length<0||off>b->length-length)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(length==0)return 0;if(eof_)return -1;
 while(true){try{const auto count=inf->inflate(b,off,length);if(count>0)return count;if(inf->finished()||inf->needsDictionary()){eof_=true;return -1;}if(inf->needsInput())fill();}catch(const DataFormatException& e){throw ZipException(e.what());}}
}
void InflaterInputStream::resetInflationState_() noexcept {
    eof_ = false;
    len = 0;
}

void InflaterInputStream::fill(){ensureOpen_();len=in_->read(buf,0,buf->length);if(len==-1)throw ::jxx::io::EOFException("Unexpected end of ZLIB input stream");inf->setInput(buf,0,len);}
::jxx::lang::jlong InflaterInputStream::skip(::jxx::lang::jlong count){ensureOpen_();if(count<0)throw ::jxx::lang::IllegalArgumentException();auto temp=::jxx::NEW<::jxx::lang::ByteArrayType>(512);::jxx::lang::jlong total=0;while(total<count){const auto request=static_cast<::jxx::lang::jint>(std::min<::jxx::lang::jlong>(count-total,temp->length));const auto n=read(temp,0,request);if(n==-1)break;total+=n;}return total;}
::jxx::lang::jint InflaterInputStream::available(){ensureOpen_();return eof_?0:1;}
void InflaterInputStream::close(){if(closed_)return;if(usesDefaultInflater_)inf->end();in_->close();closed_=true;}
::jxx::lang::jbool InflaterInputStream::markSupported()const{return false;}
void InflaterInputStream::mark(::jxx::lang::jint){}
void InflaterInputStream::reset(){throw ::jxx::io::IOException("mark/reset not supported");}
} // namespace jxx::util::zip
