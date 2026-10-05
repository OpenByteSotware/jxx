#include "util/zip/jxx.util.zip.GZIPOutputStream.h"
#include <zlib.h>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::util::zip {namespace {constexpr ::jxx::lang::jint DEFAULT_SIZE=512;}
GZIPOutputStream::GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o):GZIPOutputStream(o,DEFAULT_SIZE,false){}
GZIPOutputStream::GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o,::jxx::lang::jint s):GZIPOutputStream(o,s,false){}
GZIPOutputStream::GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o,::jxx::lang::jbool f):GZIPOutputStream(o,DEFAULT_SIZE,f){}
GZIPOutputStream::GZIPOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o,::jxx::lang::jint s,::jxx::lang::jbool f):Super(o,::jxx::NEW<Deflater>(Deflater::DEFAULT_COMPRESSION,true),s,f){crc_=static_cast<std::uint32_t>(::crc32(0L,Z_NULL,0));writeHeader_();}
GZIPOutputStream::~GZIPOutputStream(){if(def)def->end();}
void GZIPOutputStream::ensureGzipOpen_()const{if(gzipClosed_)throw ::jxx::io::IOException("stream closed");}
void GZIPOutputStream::writeHeader_(){static const unsigned char h[10]={0x1f,0x8b,8,0,0,0,0,0,0,0};for(auto v:h)out_->write(v);}
void GZIPOutputStream::write(::jxx::lang::jint v){auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(1);(*a)[0]=static_cast<::jxx::lang::jbyte>(v);write(a,0,1);}
void GZIPOutputStream::write(const ::jxx::lang::ByteArray&b){if(!b)throw ::jxx::lang::NullPointerException();write(b,0,b->length);}
void GZIPOutputStream::write(const ::jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){ensureGzipOpen_();if(!b)throw ::jxx::lang::NullPointerException();if(o<0||n<0||o>b->length-n)throw ::jxx::lang::ArrayIndexOutOfBoundsException();if(n==0)return;crc_=static_cast<std::uint32_t>(::crc32(crc_,reinterpret_cast<const Bytef*>(&(*b)[o]),static_cast<uInt>(n)));size_+=static_cast<std::uint32_t>(n);DeflaterOutputStream::write(b,o,n);}
void GZIPOutputStream::writeTrailer_(){for(int i=0;i<4;++i)out_->write(static_cast<::jxx::lang::jint>((crc_>>(8*i))&0xffU));for(int i=0;i<4;++i)out_->write(static_cast<::jxx::lang::jint>((size_>>(8*i))&0xffU));}
void GZIPOutputStream::finish(){ensureGzipOpen_();if(gzipFinished_)return;DeflaterOutputStream::finish();writeTrailer_();gzipFinished_=true;}
void GZIPOutputStream::close(){if(gzipClosed_)return;try{finish();out_->close();}catch(...){gzipClosed_=true;def->end();throw;}gzipClosed_=true;def->end();}
}
